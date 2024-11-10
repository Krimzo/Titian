#include "titian.h"


titian::CPUTracer::CPUTracer( kl::Window& window, TracingScene const& scene, Int2 resolution )
    : window( window ), scene( scene )
{
    m_target.resize( resolution );
    m_tasks.resize( kl::CPU_CORE_COUNT );
}

void titian::CPUTracer::render()
{
    auto _ = std::async( std::launch::async, [this] { render_scene(); } );
    kl::Image frame;
    while ( window.process() )
    {
        handle_input();
        make_frame( frame );
        window.draw_image( frame );
    }
}

void titian::CPUTracer::handle_input()
{
    if ( window.keyboard.esc )
        window.close();

    if ( window.keyboard.ctrl && window.keyboard.s.pressed() )
    {
        if ( auto path = kl::choose_file( true, { { "Images", ".png" } } ) )
        {
            if ( !path->ends_with( ".png" ) )
                path->append( ".png" );

            m_target.save_to_file( path.value(), kl::ImageType::PNG );
        }
    }
}

void titian::CPUTracer::make_frame( kl::Image& frame )
{
    RGB special_color = GUILayer::get().special_color;
    frame = m_target;
    for ( auto& task : m_tasks )
    {
        auto& [sqr_pos, sqr_size] = task.section;
        frame.draw_rectangle( sqr_pos, sqr_pos + sqr_size, special_color, false );
    }
    frame.resize_scaled( window.size() );
}

void titian::CPUTracer::render_scene()
{
    int square_size = kl::clamp( kl::min( m_target.width(), m_target.height() ) / (int) m_tasks.size(), 8, 256 );
    Int2 square_counts = {
        (int) ceil( m_target.width() / (float) square_size ),
        (int) ceil( m_target.height() / (float) square_size ),
    };
    std::atomic_int32_t id_counter = 0;
    for ( auto& task : m_tasks )
    {
        task.task = std::async( std::launch::async, [&]
        {
            while ( true )
            {
                int32_t id = id_counter++;
                if ( id >= (square_counts.x * square_counts.y) || !window.active() )
                    break;

                Int2 sqr_pos = {
                    id % square_counts.x * square_size,
                    id / square_counts.x * square_size,
                };
                task.section = { sqr_pos, Int2( square_size ) };
                render_section( sqr_pos, Int2( square_size ) );
            }
            task.section = { Int2( -1 ), Int2( -1 ) };
        } );
    }
    for ( auto& task : m_tasks )
        task.task.wait();
}

void titian::CPUTracer::render_section( Int2 top_left, Int2 size )
{
    Int2 incl_start = kl::max( top_left, Int2( 0 ) );
    Int2 excl_end = kl::min( top_left + size, m_target.size() );
    for ( int y = incl_start.y; y < excl_end.y; y++ )
    {
        for ( int x = incl_start.x; x < excl_end.x; x++ )
        {
            Float2 ndc = {
                float( x ) / (m_target.width() - 1) * 2.0f - 1.0f,
                float( m_target.height() - 1 - y ) / (m_target.height() - 1) * 2.0f - 1.0f,
            };
            m_target[{ x, y }] = render_pixel( ndc );
        }
    }
}

titian::RGB titian::CPUTracer::render_pixel( Float2 ndc )
{
    if ( !scene.camera )
        return RGB{};

    TracingCamera const& camera = *scene.camera;
    kl::Ray ray{ camera.position, camera.inv_mat, ndc };
    if ( camera.wireframe )
        return camera.sample_background( ray.direction() );

    Float3 color;
    for ( int i = 0; i < Tracing::ACCUMULATION_LIMIT; i++ )
        color += trace_ray( ray, 0, nullptr );

    return color * (1.0f / Tracing::ACCUMULATION_LIMIT);
}

titian::Float3 titian::CPUTracer::trace_ray( kl::Ray const& ray, int depth, kl::Triangle const* blacklist )
{
    if ( depth > Tracing::DEPTH_LIMIT )
        return {};

    Opt<TracingPayload> payload = scene.trace( ray, blacklist );
    if ( !payload )
    {
        Float3 light = scene.camera->sample_background( ray.direction() );
        if ( scene.directional )
        {
            float diffuse = kl::clamp( kl::dot( -scene.directional->direction, ray.direction() ), 0.0f, 1.0f );
            light += scene.directional->color * scene.directional->point_size * diffuse;
        }
        return kl::clamp( light, (Float3) 0.0f, (Float3) 1.0f );
    }

    auto& material = payload->entity.material;
    Float3 interp_weights = payload->triangle.weights( payload->intersect );
    kl::Vertex interp_vertex = payload->triangle.interpolate_self( interp_weights );

    Float3 normal = interp_vertex.normal;
    if ( material.normal_texture )
    {
        Float3 tex_norm = material.normal_texture->image.sample( interp_vertex.uv );
        tex_norm = kl::normalize( tex_norm * 2.0f - Float3( 1.0f ) );
        normal = kl::normalize( interp_vertex.normal - tex_norm );
    }
    if ( kl::dot( -ray.direction(), normal ) < 0.0f )
        normal = -normal;

    Float3 light;
    if ( material.reflectivity_factor >= 0.0f )
    {
        Float3 random_dir = kl::normalize( rand_float3( normal ) );
        float random_infl = 1.0f - kl::clamp( material.reflectivity_factor, 0.0f, 1.0f );
        Float3 random_norm = kl::normalize( normal + random_dir * random_infl );
        Float3 reflect_dir = kl::reflect( ray.direction(), random_norm );
        kl::Ray reflection_ray{ payload->intersect, reflect_dir };
        light += trace_ray( reflection_ray, depth + 1, &payload->triangle );
    }
    else
    {
        Float3 random_dir = kl::normalize( rand_float3( normal ) );
        float random_infl = 1.0f - kl::clamp( -material.reflectivity_factor, 0.0f, 1.0f );
        Float3 random_norm = kl::normalize( normal + random_dir * random_infl );
        Float3 refract_dir = kl::refract( ray.direction(), random_norm, 1.0f / material.refraction_index );
        kl::Ray refraction_ray{ payload->intersect, refract_dir };
        light += trace_ray( refraction_ray, depth + 1, &payload->triangle );
    }

    Float3 color = material.color;
    if ( material.color_texture )
    {
        Float3 tex_col = material.color_texture->image.sample( interp_vertex.uv );
        color = kl::lerp( material.texture_blend, color, tex_col );
    }
    return color * light;
}

titian::Float3 titian::CPUTracer::align_to_normal( Float3 const& sample, Float3 const& normal )
{
    Float3 tangent = abs( normal.x ) > 0.1f ? Float3( 0.0f, 1.0f, 0.0f ) : Float3( 1.0f, 0.0f, 0.0f );
    tangent = kl::normalize( kl::cross( normal, tangent ) );
    Float3 bitangent = kl::cross( normal, tangent );
    return tangent * sample.x + bitangent * sample.y + normal * sample.z;
}

titian::Float3 titian::CPUTracer::rand_float3( Float3 const& normal )
{
    float u = kl::random::gen_float( 0.0f, 1.0f );
    float v = kl::random::gen_float( 0.0f, 1.0f );
    float theta = 2.0f * kl::pi() * u;
    float r = sqrt( 1.0f - v );
    float x = r * cos( theta );
    float y = r * sin( theta );
    float z = sqrt( v );
    Float3 sample = { x, y, z };
    return align_to_normal( sample, normal );
}
