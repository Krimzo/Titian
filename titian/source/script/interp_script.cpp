#include "titian.h"


titian::InterpScript::InterpScript()
{
    source = get_default_script();
    reload();
}

void titian::InterpScript::serialize( Serializer& serializer ) const
{
    Script::serialize( serializer );
    serializer.write_string( "source", source );
}

void titian::InterpScript::deserialize( Serializer const& serializer )
{
    Script::deserialize( serializer );
    serializer.read_string( "source", source );
    reload();
}

bool titian::InterpScript::is_valid() const
{
    return m_engine;
}

void titian::InterpScript::reload()
{
    m_start_function = new sl::function();
    m_update_function = new sl::function();
    m_collision_function = new sl::function();
    m_ui_function = new sl::function();

    m_engine = new sl::state();
    m_engine->open_libraries( sl::lib::base, sl::lib::package, sl::lib::string, sl::lib::math, sl::lib::table );
    load_engine_parts();

    try
    {
        m_engine->script( source );
    }
    catch ( Except const& e )
    {
        Logger::log( e.what() );
    }

    *m_start_function = (*m_engine)["on_start"];
    *m_update_function = (*m_engine)["on_update"];
    *m_collision_function = (*m_engine)["on_collision"];
    *m_ui_function = (*m_engine)["on_ui"];
}

void titian::InterpScript::call_start( Scene& scene )
{
    if ( !m_start_function->valid() )
        return;

    auto result = m_start_function->call( scene );
    if ( !result.valid() )
    {
        sol::error err = result;
        Logger::log( "On start error: ", err.what() );
    }
}

void titian::InterpScript::call_update( Scene& scene )
{
    if ( !m_update_function->valid() )
        return;

    auto result = m_update_function->call( scene );
    if ( !result.valid() )
    {
        sol::error err = result;
        Logger::log( "On update error: ", err.what() );
    }
}

void titian::InterpScript::call_collision( Scene& scene, Entity& attacker, Entity& target )
{
    if ( !m_collision_function->valid() )
        return;

    auto result = m_collision_function->call( scene, attacker, target );
    if ( !result.valid() )
    {
        sol::error err = result;
        Logger::log( "On collision error: ", err.what() );
    }
}

void titian::InterpScript::call_ui( Scene& scene )
{
    if ( !m_ui_function->valid() )
        return;

    auto result = m_ui_function->call( scene );
    if ( !result.valid() )
    {
        sol::error err = result;
        Logger::log( "On ui error: ", err.what() );
    }
}

titian::StringMap<titian::InterpScript::Parameter> titian::InterpScript::get_parameters()
{
    if ( !m_engine )
        return {};

    StringMap<Parameter> parameters;
    for ( auto& [key, value] : m_engine->globals() )
    {
        if ( !key.is<String>() )
            continue;

        String name = key.as<String>();
        if ( !name.starts_with( "p_" ) )
            continue;

        parameters.emplace( name.substr( 2 ), Parameter{ *this, name } );
    }
    return parameters;
}

#define FUNCTION(R, F, ...) (static_cast<R (*)(__VA_ARGS__)>(&F))
#define METHOD(T, R, F, ...) (static_cast<R (T::*)(__VA_ARGS__)>(&T::F))
#define CONST_METHOD(T, R, F, ...) (static_cast<R (T::*)(__VA_ARGS__) const>(&T::F))

void titian::InterpScript::load_engine_parts()
{
    m_engine->new_usertype<Int2>(
        "Int2",
        sl::constructors<Int2(), Int2( int ), Int2( int, int )>(),
        "x", &Int2::x,
        "y", &Int2::y,
        sl::meta_function::equal_to, &Int2::operator==,
        sl::meta_function::unary_minus, CONST_METHOD( Int2, Int2, operator- ),
        sl::meta_function::addition, &Int2::operator+,
        sl::meta_function::subtraction, CONST_METHOD( Int2, Int2, operator-, Int2 const& ),
        sl::meta_function::multiplication, sl::overload( CONST_METHOD( Int2, Int2, operator*, int ), CONST_METHOD( Int2, Int2, operator*, Int2 const& ) ),
        sl::meta_function::division, sl::overload( CONST_METHOD( Int2, Int2, operator/, int ), CONST_METHOD( Int2, Int2, operator/, Int2 const& ) )
    );

    m_engine->new_usertype<Float2>(
        "Float2",
        sl::constructors<Float2(), Float2( float ), Float2( float, float )>(),
        "x", &Float2::x,
        "y", &Float2::y,
        "length", &Float2::length,
        sl::meta_function::equal_to, &Float2::operator==,
        sl::meta_function::unary_minus, CONST_METHOD( Float2, Float2, operator- ),
        sl::meta_function::addition, &Float2::operator+,
        sl::meta_function::subtraction, CONST_METHOD( Float2, Float2, operator-, Float2 const& ),
        sl::meta_function::multiplication, sl::overload( CONST_METHOD( Float2, Float2, operator*, float ), CONST_METHOD( Float2, Float2, operator*, Float2 const& ) ),
        sl::meta_function::division, sl::overload( CONST_METHOD( Float2, Float2, operator/, float ), CONST_METHOD( Float2, Float2, operator/, Float2 const& ) )
    );

    m_engine->new_usertype<Float3>(
        "Float3",
        sl::constructors<Float3(), Float3( float ), Float3( float, float, float ), Float3( Float2, float ), Float3( float, Float2 )>(),
        "x", &Float3::x,
        "y", &Float3::y,
        "z", &Float3::z,
        "length", &Float3::length,
        sl::meta_function::equal_to, &Float3::operator==,
        sl::meta_function::unary_minus, CONST_METHOD( Float3, Float3, operator- ),
        sl::meta_function::addition, &Float3::operator+,
        sl::meta_function::subtraction, CONST_METHOD( Float3, Float3, operator-, Float3 const& ),
        sl::meta_function::multiplication, sl::overload( CONST_METHOD( Float3, Float3, operator*, float ), CONST_METHOD( Float3, Float3, operator*, Float3 const& ) ),
        sl::meta_function::division, sl::overload( CONST_METHOD( Float3, Float3, operator/, float ), CONST_METHOD( Float3, Float3, operator/, Float3 const& ) )
    );

    m_engine->new_usertype<Float4>(
        "Float4",
        sl::constructors<Float4(), Float4( float ), Float4( float, float, float, float ), Float4( Float2, float, float ), Float4( float, Float2, float ), Float4( float, float, Float2 ), Float4( Float2, Float2 ), Float4( Float3, float ), Float4( float, Float3 )>(),
        "x", &Float4::x,
        "y", &Float4::y,
        "z", &Float4::z,
        "w", &Float4::w,
        "length", &Float4::length,
        sl::meta_function::equal_to, &Float4::operator==,
        sl::meta_function::unary_minus, CONST_METHOD( Float4, Float4, operator- ),
        sl::meta_function::addition, &Float4::operator+,
        sl::meta_function::subtraction, CONST_METHOD( Float4, Float4, operator-, Float4 const& ),
        sl::meta_function::multiplication, sl::overload( CONST_METHOD( Float4, Float4, operator*, float ), CONST_METHOD( Float4, Float4, operator*, Float4 const& ) ),
        sl::meta_function::division, sl::overload( CONST_METHOD( Float4, Float4, operator/, float ), CONST_METHOD( Float4, Float4, operator/, Float4 const& ) )
    );

    m_engine->new_usertype<Float2x2>(
        "Float2x2",
        sl::constructors<Float2x2()>(),
        "get", &Float2x2::get,
        "set", &Float2x2::set,
        "determinant", &Float2x2::determinant,
        sl::meta_function::equal_to, &Float2x2::operator==,
        sl::meta_function::addition, &Float2x2::operator+,
        sl::meta_function::subtraction, &Float2x2::operator-,
        sl::meta_function::multiplication, sl::overload( CONST_METHOD( Float2x2, Float2x2, operator*, float ), CONST_METHOD( Float2x2, Float2, operator*, Float2 const& ), CONST_METHOD( Float2x2, Float2x2, operator*, Float2x2 const& ) )
    );

    m_engine->new_usertype<Float3x3>(
        "Float3x3",
        sl::constructors<Float3x3()>(),
        "get", &Float3x3::get,
        "set", &Float3x3::set,
        "determinant", &Float3x3::determinant,
        sl::meta_function::equal_to, &Float3x3::operator==,
        sl::meta_function::addition, &Float3x3::operator+,
        sl::meta_function::subtraction, &Float3x3::operator-,
        sl::meta_function::multiplication, sl::overload( CONST_METHOD( Float3x3, Float3x3, operator*, float ), CONST_METHOD( Float3x3, Float3, operator*, Float3 const& ), CONST_METHOD( Float3x3, Float3x3, operator*, Float3x3 const& ) )
    );

    m_engine->new_usertype<Float4x4>(
        "Float4x4",
        sl::constructors<Float4x4()>(),
        "get", &Float4x4::get,
        "set", &Float4x4::set,
        "determinant", &Float4x4::determinant,
        sl::meta_function::equal_to, &Float4x4::operator==,
        sl::meta_function::addition, &Float4x4::operator+,
        sl::meta_function::subtraction, &Float4x4::operator-,
        sl::meta_function::multiplication, sl::overload( CONST_METHOD( Float4x4, Float4x4, operator*, float ), CONST_METHOD( Float4x4, Float4, operator*, Float4 const& ), CONST_METHOD( Float4x4, Float4x4, operator*, Float4x4 const& ) )
    );

    m_engine->new_usertype<Complex>(
        "Complex",
        sl::constructors<Complex(), Complex( float ), Complex( float, float )>(),
        "r", &Complex::r,
        "i", &Complex::i,
        "length", &Complex::length,
        sl::meta_function::equal_to, &Complex::operator==,
        sl::meta_function::unary_minus, CONST_METHOD( Complex, Complex, operator- ),
        sl::meta_function::addition, &Complex::operator+,
        sl::meta_function::subtraction, CONST_METHOD( Complex, Complex, operator-, Complex const& ),
        sl::meta_function::multiplication, sl::overload( CONST_METHOD( Complex, Complex, operator*, float ), CONST_METHOD( Complex, Complex, operator*, Complex const& ) )
    );

    m_engine->new_usertype<Quaternion>(
        "Quaternion",
        sl::constructors<Quaternion(), Quaternion( float, float, float ), Quaternion( float, float, float, float ), Quaternion( Float3, float )>(),
        "w", &Quaternion::w,
        "x", &Quaternion::x,
        "y", &Quaternion::y,
        "z", &Quaternion::z,
        "length", &Quaternion::length,
        sl::meta_function::equal_to, &Quaternion::operator==,
        sl::meta_function::unary_minus, CONST_METHOD( Quaternion, Quaternion, operator- ),
        sl::meta_function::addition, &Quaternion::operator+,
        sl::meta_function::subtraction, CONST_METHOD( Quaternion, Quaternion, operator-, Quaternion const& ),
        sl::meta_function::multiplication, sl::overload( CONST_METHOD( Quaternion, Quaternion, operator*, float ), CONST_METHOD( Quaternion, Quaternion, operator*, Quaternion const& ) )
    );

    m_engine->new_usertype<kl::Vertex>(
        "Vertex",
        sl::constructors<kl::Vertex(), kl::Vertex( Float3 ), kl::Vertex( Float3, Float3, Float2 )>(),
        "position", &kl::Vertex::position,
        "normal", &kl::Vertex::normal,
        "uv", &kl::Vertex::uv
    );

    m_engine->new_usertype<kl::Triangle>(
        "Triangle",
        sl::constructors<kl::Triangle(), kl::Triangle( kl::Vertex, kl::Vertex, kl::Vertex )>(),
        "a", &kl::Triangle::a,
        "b", &kl::Triangle::b,
        "c", &kl::Triangle::c
    );

    m_engine->new_usertype<kl::Plane>(
        "Plane",
        sl::constructors<kl::Plane(), kl::Plane( Float3, Float3 )>(),
        "position", &kl::Plane::position,
        "normal", sl::overload( &kl::Plane::normal, &kl::Plane::set_normal ),
        "in_front", &kl::Plane::in_front
    );

    m_engine->new_usertype<kl::Sphere>(
        "Sphere",
        sl::constructors<kl::Sphere(), kl::Sphere( Float3, float )>(),
        "position", &kl::Sphere::position,
        "radius", &kl::Sphere::radius
    );

    m_engine->new_usertype<kl::AABB>(
        "AABB",
        sl::constructors<kl::AABB(), kl::AABB( Float3, Float3 )>(),
        "position", &kl::AABB::position,
        "size", &kl::AABB::size
    );

    m_engine->new_usertype<kl::Ray>(
        "Ray",
        sl::constructors<kl::Ray(), kl::Ray( Float3, Float3 ), kl::Ray( Float3, Float4x4, Float2 )>(),
        "origin", &kl::Ray::origin,
        "direction", sl::property( &kl::Ray::direction, &kl::Ray::set_direction ),
        "intersect_plane", &kl::Ray::intersect_plane,
        "intersect_sphere", &kl::Ray::intersect_sphere,
        "intersect_aabb", &kl::Ray::intersect_aabb,
        "intersect_triangle", &kl::Ray::intersect_triangle
    );

    m_engine->new_usertype<RGB>(
        "RGB",
        sl::constructors<RGB(), RGB( byte, byte, byte ), RGB( byte, byte, byte, byte )>(),
        "r", &RGB::r,
        "g", &RGB::g,
        "b", &RGB::b,
        "a", &RGB::a,
        "gray", &RGB::gray,
        "inverted", &RGB::inverted,
        "ascii", &RGB::ascii,
        "mix", sl::overload( CONST_METHOD( RGB, RGB, mix, RGB, float ), CONST_METHOD( RGB, RGB, mix, RGB ) ),
        sl::meta_function::equal_to, &RGB::operator==
    );

    m_engine->new_usertype<YUV>(
        "YUV",
        sl::constructors<YUV(), YUV( float, float, float )>(),
        "y", &YUV::y,
        "u", &YUV::u,
        "v", &YUV::v,
        "ascii", &YUV::ascii,
        sl::meta_function::equal_to, &YUV::operator==
    );

    m_engine->new_usertype<kl::Image>(
        "Image",
        sl::constructors<kl::Image(), kl::Image( Int2 ), kl::Image( StringRef )>(),
        "pixel_count", &kl::Image::pixel_count,
        "byte_size", &kl::Image::byte_size,
        sl::meta_function::index, sl::overload( METHOD( kl::Image, RGB&, operator[], int ), METHOD( kl::Image, RGB&, operator[], Int2 ) ),
        sl::meta_function::new_index, sl::overload( METHOD( kl::Image, RGB&, operator[], int ), METHOD( kl::Image, RGB&, operator[], Int2 ) ),
        "in_bounds", &kl::Image::in_bounds,
        "sample", &kl::Image::sample,
        "width", sl::property( &kl::Image::width, &kl::Image::set_width ),
        "height", sl::property( &kl::Image::height, &kl::Image::set_height ),
        "size", sl::property( &kl::Image::size, &kl::Image::resize ),
        "fill", &kl::Image::fill,
        "flip_horizontal", &kl::Image::flip_horizontal,
        "flip_vertical", &kl::Image::flip_vertical,
        "rectangle", &kl::Image::rectangle,
        "as_ascii", &kl::Image::as_ascii,
        "draw_line", &kl::Image::draw_line,
        "draw_triangle", &kl::Image::draw_triangle,
        "draw_rectangle", &kl::Image::draw_rectangle,
        "draw_circle", sl::overload( METHOD( kl::Image, void, draw_circle, Int2, float, RGB, bool ), METHOD( kl::Image, void, draw_circle, Int2, Int2, RGB, bool ) ),
        "draw_image", &kl::Image::draw_image,
        "load_from_file", &kl::Image::load_from_file,
        "save_to_file", &kl::Image::save_to_file
    );

    m_engine->new_usertype<Mesh>(
        "Mesh",
        "vertices", &Mesh::vertices,
        "topology", &Mesh::topology,
        "render_wireframe", &Mesh::render_wireframe,
        "load_vertices", &Mesh::load_vertices,
        "load_triangles", &Mesh::load_triangles,
        "reload", &Mesh::reload
    );

    m_engine->new_usertype<Animation>(
        "Animation",
        "animation_type", &Animation::animation_type,
        "ticks_per_second", &Animation::ticks_per_second,
        "duration_in_ticks", &Animation::duration_in_ticks,
        "meshes", &Animation::meshes,
        "get_index", &Animation::get_index,
        "get_mesh", &Animation::get_mesh,
        "update", &Animation::update
    );

    m_engine->new_usertype<Texture>(
        "Texture",
        "image", &Texture::image,
        "is_cube", &Texture::is_cube,
        "reload_as_2D", &Texture::reload_as_2D,
        "reload_as_cube", &Texture::reload_as_cube,
        "create_target_view", &Texture::create_target_view,
        "create_depth_view", &Texture::create_depth_view,
        "create_shader_view", &Texture::create_shader_view,
        "create_access_view", &Texture::create_access_view,
        "resolution", &Texture::resolution
    );

    m_engine->new_usertype<Material>(
        "Material",
        "texture_blend", &Material::texture_blend,
        "reflectivity_factor", &Material::reflectivity_factor,
        "refraction_index", &Material::refraction_index,
        "color", &Material::color,
        "custom_data", &Material::custom_data,
        "color_texture_name", &Material::color_texture_name,
        "normal_texture_name", &Material::normal_texture_name,
        "roughness_texture_name", &Material::roughness_texture_name,
        "shader_name", &Material::shader_name
    );

    m_engine->new_usertype<Shader>(
        "Shader",
        "shader_type", &Shader::shader_type,
        "source", &Shader::source,
        "reload", &Shader::reload
    );

    m_engine->new_usertype<Entity>(
        "Entity",
        "shadows", &Entity::shadows,
        "animation_name", &Entity::animation_name,
        "material_name", &Entity::material_name,
        "dynamic", sl::property( &Entity::dynamic, &Entity::set_dynamic ),
        "gravity", sl::property( &Entity::gravity, &Entity::set_gravity ),
        "mass", sl::property( &Entity::mass, &Entity::set_mass ),
        "angular_damping", sl::property( &Entity::angular_damping, &Entity::set_angular_damping ),
        "static_friction", sl::property( &Entity::static_friction, &Entity::set_static_friction ),
        "dynamic_friction", sl::property( &Entity::dynamic_friction, &Entity::set_dynamic_friction ),
        "restitution", sl::property( &Entity::restitution, &Entity::set_restitution ),
        "scale", sl::property( &Entity::scale, &Entity::set_scale ),
        "rotation", sl::property( &Entity::rotation, &Entity::set_rotation ),
        "position", sl::property( &Entity::position, &Entity::set_position ),
        "velocity", sl::property( &Entity::velocity, &Entity::set_velocity ),
        "angular", sl::property( &Entity::angular, &Entity::set_angular ),
        "collider_rotation", sl::property( &Entity::collider_rotation, &Entity::set_collider_rotation ),
        "collider_offset", sl::property( &Entity::collider_offset, &Entity::set_collider_offset ),
        "set_box_collider", &Entity::set_box_collider,
        "set_sphere_collider", &Entity::set_sphere_collider,
        "set_capsule_collider", &Entity::set_capsule_collider,
        "remove_collider", &Entity::remove_collider,
        "model_matrix", &Entity::model_matrix,
        "collider_matrix", &Entity::collider_matrix
    );

    m_engine->new_usertype<Camera>(
        "Camera",
        sl::base_classes, sl::bases<Entity>(),
        "camera_type", &Camera::camera_type,
        "enabled", &Camera::enabled,
        "v_sync", &Camera::v_sync,
        "render_wireframe", &Camera::render_wireframe,
        "field_of_view", &Camera::field_of_view,
        "aspect_ratio", &Camera::aspect_ratio,
        "width", &Camera::width,
        "height", &Camera::height,
        "near_plane", &Camera::near_plane,
        "far_plane", &Camera::far_plane,
        "sensitivity", &Camera::sensitivity,
        "speed", &Camera::speed,
        "background", &Camera::background,
        "custom_data", &Camera::custom_data,
        "skybox_texture_name", &Camera::skybox_texture_name,
        "target_texture_name", &Camera::target_texture_name,
        "shader_name", &Camera::shader_name,
        "update_aspect_ratio", &Camera::update_aspect_ratio,
        "forward", sl::property( &Camera::forward, &Camera::set_forward ),
        "up", sl::property( &Camera::up, &Camera::set_up ),
        "right", &Camera::right,
        "move_forward", &Camera::move_forward,
        "move_back", &Camera::move_back,
        "move_right", &Camera::move_right,
        "move_left", &Camera::move_left,
        "move_up", &Camera::move_up,
        "move_down", &Camera::move_down,
        "rotate", &Camera::rotate,
        "view_matrix", &Camera::view_matrix,
        "projection_matrix", &Camera::projection_matrix,
        "camera_matrix", &Camera::camera_matrix,
        "resize", &Camera::resize,
        "resolution", &Camera::resolution
    );

    m_engine->new_usertype<AmbientLight>(
        "AmbientLight",
        "color", &AmbientLight::color
    );

    m_engine->new_usertype<DirectionalLight>(
        "DirectionalLight",
        "color", &DirectionalLight::color,
        "point_size", &DirectionalLight::point_size,
        "cascade_splits", &DirectionalLight::cascade_splits,
        "resolution", sl::property( &DirectionalLight::resolution, &DirectionalLight::set_resolution ),
        "direction", sl::property( &DirectionalLight::direction, &DirectionalLight::set_direction ),
        "light_matrix", &DirectionalLight::light_matrix
    );

    m_engine->new_usertype<Scene>(
        "Scene",
        "main_camera_name", &Scene::main_camera_name,
        "main_ambient_light_name", &Scene::main_ambient_light_name,
        "main_directional_light_name", &Scene::main_directional_light_name,
        "gravity", sl::property( &Scene::gravity, &Scene::set_gravity ),
        "new_mesh", &Scene::helper_new_mesh,
        "new_animation", &Scene::helper_new_animation,
        "new_texture", &Scene::helper_new_texture,
        "new_material", &Scene::helper_new_material,
        "new_shader", &Scene::helper_new_shader,
        "new_entity", &Scene::helper_new_entity,
        "get_mesh", &Scene::helper_get_mesh,
        "get_animation", &Scene::helper_get_animation,
        "get_texture", &Scene::helper_get_texture,
        "get_material", &Scene::helper_get_material,
        "get_shader", &Scene::helper_get_shader,
        "get_entity", &Scene::helper_get_entity,
        "remove_mesh", &Scene::helper_remove_mesh,
        "remove_animation", &Scene::helper_remove_animation,
        "remove_texture", &Scene::helper_remove_texture,
        "remove_material", &Scene::helper_remove_material,
        "remove_shader", &Scene::helper_remove_shader,
        "remove_entity", &Scene::helper_remove_entity,
        "contains_mesh", &Scene::helper_contains_mesh,
        "contains_animation", &Scene::helper_contains_animation,
        "contains_texture", &Scene::helper_contains_texture,
        "contains_material", &Scene::helper_contains_material,
        "contains_shader", &Scene::helper_contains_shader,
        "contains_entity", &Scene::helper_contains_entity,
        "iterate_meshes", &Scene::helper_iterate_meshes,
        "iterate_animations", &Scene::helper_iterate_animations,
        "iterate_textures", &Scene::helper_iterate_textures,
        "iterate_materials", &Scene::helper_iterate_materials,
        "iterate_shaders", &Scene::helper_iterate_shaders,
        "iterate_entities", &Scene::helper_iterate_entities,
        "get_camera", &Scene::get_casted<Camera>,
        "get_ambient_light", &Scene::get_casted<AmbientLight>,
        "get_directional_light", &Scene::get_casted<DirectionalLight>
    );

    m_engine->new_usertype<kl::Key>(
        "Key",
        "pressed", &kl::Key::pressed,
        "down", &kl::Key::operator bool,
        "released", &kl::Key::released
    );

    m_engine->new_usertype<kl::Keyboard>(
        "Keyboard",
        "q", &kl::Keyboard::q,
        "w", &kl::Keyboard::w,
        "e", &kl::Keyboard::e,
        "r", &kl::Keyboard::r,
        "t", &kl::Keyboard::t,
        "z", &kl::Keyboard::z,
        "u", &kl::Keyboard::u,
        "i", &kl::Keyboard::i,
        "o", &kl::Keyboard::o,
        "p", &kl::Keyboard::p,
        "a", &kl::Keyboard::a,
        "s", &kl::Keyboard::s,
        "d", &kl::Keyboard::d,
        "f", &kl::Keyboard::f,
        "g", &kl::Keyboard::g,
        "h", &kl::Keyboard::h,
        "j", &kl::Keyboard::j,
        "k", &kl::Keyboard::k,
        "l", &kl::Keyboard::l,
        "y", &kl::Keyboard::y,
        "x", &kl::Keyboard::x,
        "c", &kl::Keyboard::c,
        "v", &kl::Keyboard::v,
        "b", &kl::Keyboard::b,
        "n", &kl::Keyboard::n,
        "m", &kl::Keyboard::m,
        "zero", &kl::Keyboard::zero,
        "one", &kl::Keyboard::one,
        "two", &kl::Keyboard::two,
        "three", &kl::Keyboard::three,
        "four", &kl::Keyboard::four,
        "five", &kl::Keyboard::five,
        "six", &kl::Keyboard::six,
        "seven", &kl::Keyboard::seven,
        "eight", &kl::Keyboard::eight,
        "nine", &kl::Keyboard::nine,
        "period", &kl::Keyboard::period,
        "comma", &kl::Keyboard::comma,
        "plus", &kl::Keyboard::plus,
        "minus", &kl::Keyboard::minus,
        "multiply", &kl::Keyboard::multiply,
        "divide", &kl::Keyboard::divide,
        "equals", &kl::Keyboard::equals,
        "lsqrbrckt", &kl::Keyboard::lsqrbrckt,
        "rsqrbrckt", &kl::Keyboard::rsqrbrckt,
        "semicolon", &kl::Keyboard::semicolon,
        "apostrophe", &kl::Keyboard::apostrophe,
        "backslash", &kl::Keyboard::backslash,
        "backtick", &kl::Keyboard::backtick,
        "tab", &kl::Keyboard::tab,
        "space", &kl::Keyboard::space,
        "enter", &kl::Keyboard::enter,
        "backspace", &kl::Keyboard::backspace,
        "esc", &kl::Keyboard::esc,
        "caps", &kl::Keyboard::caps,
        "shift", &kl::Keyboard::shift,
        "ctrl", &kl::Keyboard::ctrl,
        "alt", &kl::Keyboard::alt,
        "insert", &kl::Keyboard::insert,
        "delet", &kl::Keyboard::delet,
        "home", &kl::Keyboard::home,
        "end", &kl::Keyboard::end,
        "pageup", &kl::Keyboard::pageup,
        "pagedown", &kl::Keyboard::pagedown,
        "up", &kl::Keyboard::up,
        "left", &kl::Keyboard::left,
        "down", &kl::Keyboard::down,
        "right", &kl::Keyboard::right,
        "f1", &kl::Keyboard::f1,
        "f2", &kl::Keyboard::f2,
        "f3", &kl::Keyboard::f3,
        "f4", &kl::Keyboard::f4,
        "f5", &kl::Keyboard::f5,
        "f6", &kl::Keyboard::f6,
        "f7", &kl::Keyboard::f7,
        "f8", &kl::Keyboard::f8,
        "f9", &kl::Keyboard::f9,
        "f10", &kl::Keyboard::f10,
        "f11", &kl::Keyboard::f11,
        "f12", &kl::Keyboard::f12
    );

    m_engine->new_usertype<kl::Mouse>(
        "Mouse",
        "left", &kl::Mouse::left,
        "middle", &kl::Mouse::middle,
        "right", &kl::Mouse::right,
        "position", sl::property( &kl::Mouse::position, &kl::Mouse::set_position ),
        "norm_position", &kl::Mouse::norm_position,
        "scroll", &kl::Mouse::scroll,
        "hidden", sl::property( &kl::Mouse::is_hidden, &kl::Mouse::set_hidden )
    );

    m_engine->new_usertype<kl::Window>(
        "Window",
        "active", &kl::Window::active,
        "close", &kl::Window::close,
        "resizeable", sl::property( &kl::Window::resizeable, &kl::Window::set_resizeable ),
        "maximize", &kl::Window::maximize,
        "minimize", &kl::Window::minimize,
        "restore", &kl::Window::restore,
        "fullscreened", sl::property( &kl::Window::fullscreened, &kl::Window::set_fullscreen ),
        "position", sl::property( &kl::Window::position, &kl::Window::set_position ),
        "size", sl::property( &kl::Window::size, &kl::Window::resize ),
        "aspect_ratio", &kl::Window::aspect_ratio,
        "frame_center", &kl::Window::frame_center,
        "set_title", &kl::Window::set_title,
        "set_icon", &kl::Window::set_icon
    );

    (*m_engine)["new_bool_vector"] = []() { return Vector<bool>{}; };
    (*m_engine)["new_int_vector"] = []() { return Vector<int>{}; };
    (*m_engine)["new_int2_vector"] = []() { return Vector<Int2>{}; };
    (*m_engine)["new_float_vector"] = []() { return Vector<float>{}; };
    (*m_engine)["new_float2_vector"] = []() { return Vector<Float2>{}; };
    (*m_engine)["new_float3_vector"] = []() { return Vector<Float3>{}; };
    (*m_engine)["new_float4_vector"] = []() { return Vector<Float4>{}; };
    (*m_engine)["new_complex_vector"] = []() { return Vector<Complex>{}; };
    (*m_engine)["new_quat_vector"] = []() { return Vector<Quaternion>{}; };
    (*m_engine)["new_vertex_vector"] = []() { return Vector<kl::Vertex>{}; };
    (*m_engine)["new_triangle_vector"] = []() { return Vector<kl::Triangle>{}; };
    (*m_engine)["new_plane_vector"] = []() { return Vector<kl::Plane>{}; };
    (*m_engine)["new_sphere_vector"] = []() { return Vector<kl::Sphere>{}; };
    (*m_engine)["new_aabb_vector"] = []() { return Vector<kl::AABB>{}; };
    (*m_engine)["new_ray_vector"] = []() { return Vector<kl::Ray>{}; };
    (*m_engine)["new_color_vector"] = []() { return Vector<RGB>{}; };
    (*m_engine)["new_string_vector"] = []() { return Vector<String>{}; };

    (*m_engine)["PI"] = kl::pi();
    (*m_engine)["TO_RADIANS"] = kl::to_radians();
    (*m_engine)["TO_DEGREES"] = kl::to_degrees();
    (*m_engine)["TO_FLOAT_RGB"] = kl::to_float_rgb();
    (*m_engine)["TO_BYTE_RGB"] = kl::to_byte_rgb();

    (*m_engine)["BLACK"] = kl::colors::BLACK;
    (*m_engine)["WHITE"] = kl::colors::WHITE;
    (*m_engine)["GRAY"] = kl::colors::GRAY;
    (*m_engine)["RED"] = kl::colors::RED;
    (*m_engine)["GREEN"] = kl::colors::GREEN;
    (*m_engine)["BLUE"] = kl::colors::BLUE;
    (*m_engine)["CYAN"] = kl::colors::CYAN;
    (*m_engine)["PURPLE"] = kl::colors::PURPLE;
    (*m_engine)["YELLOW"] = kl::colors::YELLOW;
    (*m_engine)["ORANGE"] = kl::colors::ORANGE;
    (*m_engine)["MAGENTA"] = kl::colors::MAGENTA;
    (*m_engine)["CRIMSON"] = kl::colors::CRIMSON;
    (*m_engine)["WHEAT"] = kl::colors::WHEAT;
    (*m_engine)["SKY"] = kl::colors::SKY;

    (*m_engine)["TOPOLOGY_POINTS"] = (int) D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
    (*m_engine)["TOPOLOGY_LINES"] = (int) D3D_PRIMITIVE_TOPOLOGY_LINELIST;
    (*m_engine)["TOPOLOGY_TRIANGLES"] = (int) D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    (*m_engine)["ANIMATION_SEQUENTIAL"] = (int) AnimationType::SEQUENTIAL;
    (*m_engine)["ANIMATION_SKELETAL"] = (int) AnimationType::SKELETAL;

    (*m_engine)["SHADER_MATERIAL"] = (int) ShaderType::MATERIAL;
    (*m_engine)["SHADER_CAMERA"] = (int) ShaderType::CAMERA;

    (*m_engine)["CAMERA_PERSPECTIVE"] = (int) CameraType::PERSPECTIVE;
    (*m_engine)["CAMERA_ORTHOGRAPHIC"] = (int) CameraType::ORTHOGRAPHIC;

    (*m_engine)["log"] = sl::overload(
        &Logger::log<StringRef const&>,
        &Logger::log<bool const&>,
        &Logger::log<int const&>,
        &Logger::log<Int2 const&>,
        &Logger::log<float const&>,
        &Logger::log<Float2 const&>,
        &Logger::log<Float3 const&>,
        &Logger::log<Float4 const&>,
        &Logger::log<Complex const&>,
        &Logger::log<Quaternion const&>,
        &Logger::log<kl::Vertex const&>,
        &Logger::log<kl::Triangle const&>,
        &Logger::log<kl::Plane const&>,
        &Logger::log<kl::Sphere const&>,
        &Logger::log<kl::AABB const&>,
        &Logger::log<kl::Ray const&> );
    (*m_engine)["print"] = (*m_engine)["log"];

    (*m_engine)["elapsed_t"] = [] { return AppLayer::get().timer.elapsed(); };
    (*m_engine)["delta_t"] = [] { return AppLayer::get().timer.delta(); };

    (*m_engine)["get_window"] = []() -> kl::Window& { return AppLayer::get().window; };
    (*m_engine)["get_keyboard"] = []() -> kl::Keyboard& { return AppLayer::get().window.keyboard; };
    (*m_engine)["get_mouse"] = []() -> kl::Mouse& { return AppLayer::get().window.mouse; };

    (*m_engine)["Float3x3_translation"] = &Float3x3::translation;
    (*m_engine)["Float3x3_rotation"] = &Float3x3::rotation;
    (*m_engine)["Float3x3_scaling"] = &Float3x3::scaling;

    (*m_engine)["Float4x4_translation"] = &Float4x4::translation;
    (*m_engine)["Float4x4_rotation"] = &Float4x4::rotation;
    (*m_engine)["Float4x4_scaling"] = &Float4x4::scaling;
    (*m_engine)["Float4x4_look_at"] = &Float4x4::look_at;
    (*m_engine)["Float4x4_perspective"] = &Float4x4::perspective;
    (*m_engine)["Float4x4_orthographic"] = &Float4x4::orthographic;

    (*m_engine)["sin"] = &kl::sin<float>;
    (*m_engine)["cos"] = &kl::cos<float>;
    (*m_engine)["tan"] = &kl::tan<float>;

    (*m_engine)["asin"] = &kl::asin<float>;
    (*m_engine)["acos"] = &kl::acos<float>;
    (*m_engine)["atan"] = &kl::atan<float>;

    (*m_engine)["sin_d"] = &kl::sin_d<float>;
    (*m_engine)["cos_d"] = &kl::cos_d<float>;
    (*m_engine)["tan_d"] = &kl::tan_d<float>;

    (*m_engine)["asin_d"] = &kl::asin_d<float>;
    (*m_engine)["acos_d"] = &kl::acos_d<float>;
    (*m_engine)["atan_d"] = &kl::atan_d<float>;

    (*m_engine)["calc_ndc"] = &kl::calc_ndc<float>;
    (*m_engine)["calc_ndc_ar"] = &kl::calc_ndc_ar<float>;

    (*m_engine)["line_x"] = &kl::line_x<float>;
    (*m_engine)["line_y"] = &kl::line_y<float>;

    (*m_engine)["clamp"] = FUNCTION( float, kl::clamp, float, float, float );
    (*m_engine)["lerp"] = FUNCTION( float, kl::lerp, float, float, float );
    (*m_engine)["unlerp"] = FUNCTION( float, kl::unlerp, float, float, float );

    (*m_engine)["to_quat"] = sl::overload(
        FUNCTION( Quaternion, kl::to_quat, Float3 const& ),
        FUNCTION( Quaternion, kl::to_quat, Float3 const&, Float3 const& )
    );

    (*m_engine)["to_euler"] = sl::overload(
        FUNCTION( Float3, kl::to_euler, Quaternion const& ),
        FUNCTION( Float3, kl::to_euler, Float3 const&, Float3 const& )
    );

    (*m_engine)["abs"] = sl::overload(
        FUNCTION( int, kl::abs, int ),
        FUNCTION( Int2, kl::abs, Int2 const& ),
        FUNCTION( float, kl::abs, float ),
        FUNCTION( Float2, kl::abs, Float2 const& ),
        FUNCTION( Float3, kl::abs, Float3 const& ),
        FUNCTION( Float4, kl::abs, Float4 const& ),
        FUNCTION( Complex, kl::abs, Complex const& ),
        FUNCTION( Quaternion, kl::abs, Quaternion const& )
    );

    (*m_engine)["normalize"] = sl::overload(
        FUNCTION( Float2, kl::normalize, Float2 const& ),
        FUNCTION( Float3, kl::normalize, Float3 const& ),
        FUNCTION( Float4, kl::normalize, Float4 const& ),
        FUNCTION( Complex, kl::normalize, Complex const& ),
        FUNCTION( Quaternion, kl::normalize, Quaternion const& )
    );

    (*m_engine)["inverse"] = sl::overload(
        FUNCTION( Float2x2, kl::inverse, Float2x2 const& ),
        FUNCTION( Float3x3, kl::inverse, Float3x3 const& ),
        FUNCTION( Float4x4, kl::inverse, Float4x4 const& ),
        FUNCTION( Complex, kl::normalize, Complex const& ),
        FUNCTION( Quaternion, kl::inverse, Quaternion const& )
    );

    (*m_engine)["transpose"] = sl::overload(
        FUNCTION( Float2x2, kl::transpose, Float2x2 const& ),
        FUNCTION( Float3x3, kl::transpose, Float3x3 const& ),
        FUNCTION( Float4x4, kl::transpose, Float4x4 const& )
    );

    (*m_engine)["dot"] = sl::overload(
        FUNCTION( float, kl::dot, Float2 const&, Float2 const& ),
        FUNCTION( float, kl::dot, Float3 const&, Float3 const& ),
        FUNCTION( float, kl::dot, Float4 const&, Float4 const& )
    );
    (*m_engine)["cross"] = &kl::cross<float>;

    (*m_engine)["angle"] = sl::overload(
        FUNCTION( float, kl::angle, Float2, Float2, bool ),
        FUNCTION( float, kl::angle, Float3 const&, Float3 const& ),
        FUNCTION( float, kl::angle, Float4 const&, Float4 const& )
    );

    (*m_engine)["rotate"] = sl::overload(
        FUNCTION( Float2, kl::rotate, Float2 const&, float ),
        FUNCTION( Float3, kl::rotate, Float3 const&, Float3 const&, float )
    );

    (*m_engine)["reflect"] = sl::overload(
        FUNCTION( Float2, kl::reflect, Float2 const&, Float2 ),
        FUNCTION( Float3, kl::reflect, Float3 const&, Float3 )
    );

    (*m_engine)["refract"] = &kl::refract<float>;

    (*m_engine)["gen_random_bool"] = &kl::random::gen_bool;
    (*m_engine)["gen_random_int"] = FUNCTION( int, kl::random::gen_int, int, int );
    (*m_engine)["gen_random_float"] = FUNCTION( float, kl::random::gen_float, float, float );
    (*m_engine)["gen_random_float2"] = FUNCTION( Float2, kl::random::gen_float2, float, float );
    (*m_engine)["gen_random_float3"] = FUNCTION( Float3, kl::random::gen_float3, float, float );
    (*m_engine)["gen_random_float4"] = FUNCTION( Float4, kl::random::gen_float4, float, float );
    (*m_engine)["gen_random_color"] = &kl::random::gen_rgb;
    (*m_engine)["gen_random_char"] = &kl::random::gen_char;
    (*m_engine)["gen_random_string"] = &kl::random::gen_string;

    (*m_engine)["read_file"] = &kl::read_file;

    (*m_engine)["ui_separator"] = &ui_separator;
    (*m_engine)["ui_same_line"] = &ui_same_line;
    (*m_engine)["ui_set_next_width"] = &ui_set_next_width;
    (*m_engine)["ui_cursor_pos"] = &ui_cursor_pos;
    (*m_engine)["ui_set_cursor_pos"] = &ui_set_cursor_pos;
    (*m_engine)["ui_window"] = &ui_window;
    (*m_engine)["ui_button"] = &ui_button;
    (*m_engine)["ui_bool"] = &ui_bool;
    (*m_engine)["ui_int"] = &ui_int;
    (*m_engine)["ui_int2"] = &ui_int2;
    (*m_engine)["ui_float"] = &ui_float;
    (*m_engine)["ui_float2"] = &ui_float2;
    (*m_engine)["ui_float3"] = &ui_float3;
    (*m_engine)["ui_float4"] = &ui_float4;
    (*m_engine)["ui_color"] = &ui_color;
    (*m_engine)["ui_string"] = &ui_string;
    (*m_engine)["ui_text"] = &ui_text;
    (*m_engine)["ui_colored_text"] = &ui_colored_text;

    (*m_engine)["toint2"] = sl::overload(
        []( Float2 const& value ) -> Int2 { return value; }
    );
    (*m_engine)["tofloat2"] = sl::overload(
        []( Int2 const& value ) -> Float2 { return value; },
        []( Complex const& value ) -> Float2 { return value; }
    );
    (*m_engine)["tofloat3"] = sl::overload(
        []( RGB const& value ) -> Float3 { return value; },
        []( YUV const& value ) -> Float3 { return value; },
        []( Quaternion const& value ) -> Float3 { return value; }
    );
    (*m_engine)["tofloat4"] = sl::overload(
        []( RGB const& value ) -> Float4 { return value; },
        []( Quaternion const& value ) -> Float4 { return value; }
    );
    (*m_engine)["tocomplex"] = sl::overload(
        []( Float2 const& value ) -> Complex { return value; }
    );
    (*m_engine)["toquaternion"] = sl::overload(
        []( Float3 const& value ) -> Quaternion { return value; },
        []( Float4 const& value ) -> Quaternion { return value; }
    );
    (*m_engine)["torgb"] = sl::overload(
        []( YUV const& value ) -> RGB { return value; },
        []( Float3 const& value ) -> RGB { return value; },
        []( Float4 const& value ) -> RGB { return value; }
    );
    (*m_engine)["toyuv"] = sl::overload(
        []( RGB const& value ) -> YUV { return value; },
        []( Float3 const& value ) -> YUV { return value; }
    );
}

static int load_names = [&]
{
    using namespace titian;

    LUA_KEYWORDS = {
        "and",
        "break",
        "do",
        "else",
        "elseif",
        "end",
        "false",
        "for",
        "function",
        "goto",
        "if",
        "in",
        "local",
        "nil",
        "not",
        "or",
        "repeat",
        "return",
        "then",
        "true",
        "until",
        "while",
    };

    InterpScript temp{};
    for ( auto& [key, value] : temp.get_engine().globals() )
    {
        if ( !key.is<String>() )
            continue;

        String name = key.as<String>();
        auto type = value.get_type();
        if ( type == sl::type::function )
        {
            LUA_FUNCTIONS.insert( name );
        }
        else
        {
            LUA_TYPES.insert( name );
        }
    }
    for ( auto& [key, value] : temp.get_engine().registry() )
    {
        if ( !key.is<std::string>() )
            continue;

        std::string name = key.as<std::string>();
        if ( name.starts_with( '_' ) )
            continue;

        if ( !value.is<sl::table>() )
            continue;
        sol::table tbl = value;

        for ( auto& [key, value] : tbl )
        {
            if ( !key.is<std::string>() )
                continue;

            std::string name = key.as<std::string>();
            if ( name.starts_with( '_' ) )
                continue;

            LUA_MEMBERS.insert( name );
        }
    }

    constexpr auto name_eraser = []( String const& name ) -> bool
    {
        return name.end() != std::find_if_not( name.begin(), name.end(), []( char c )
        {
            return std::isalnum( c ) || c == '_';
        } );
    };
    std::erase_if( LUA_TYPES, name_eraser );
    std::erase_if( LUA_MEMBERS, name_eraser );
    std::erase_if( LUA_FUNCTIONS, name_eraser );
    std::erase_if( LUA_KEYWORDS, name_eraser );

    return 0;
}();
