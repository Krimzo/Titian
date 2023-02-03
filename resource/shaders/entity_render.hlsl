// Constants
#define SHADOW_CASCADE_COUNT 4

// Vertex shader
cbuffer VS_CB : register(b0)
{
    matrix  w_matrix;
    matrix vp_matrix;

    matrix vp_light_matrices[SHADOW_CASCADE_COUNT];
};

struct vs_out
{
    float4 screen : SV_Position;
    float3  world :    VS_World;
    float2 textur :  VS_Texture;
    float3 normal :   VS_Normal;
    
    float4 light_coords[SHADOW_CASCADE_COUNT] : VS_Light;
};

vs_out v_shader(float3 position : KL_Position, float2 textur : KL_Texture, float3 normal : KL_Normal)
{
    vs_out data;

    data.world = mul(float4(position, 1), w_matrix).xyz;
    data.screen = mul(float4(data.world, 1), vp_matrix);
    data.textur = textur;
    data.normal = mul(float4(normal, 0), w_matrix).xyz;

    for (int i = 0; i < SHADOW_CASCADE_COUNT; i++) {
        data.light_coords[i] = mul(float4(position, 1), mul(w_matrix, vp_light_matrices[i]));
        data.light_coords[i].xy *= float2(0.5f, -0.5f);
        data.light_coords[i].xy += float2(0.5f, 0.5f) * data.light_coords[i].w;
    }

    return data;
}

// Pixel shader
cbuffer PS_CB : register(b0)
{
    float4 object_color;
    float4  object_data; // (texture_blend, reflection_factor, refraction_factor, refraction_index)
    
    float4 camera_position;
    float4      light_data; // (sun.x, sun.y, sun.z, ambient_factor)

    matrix v_matrix;
    float4 cascade_distances;
};

SamplerState skybox_sampler : register(s0);
TextureCube  skybox_texture : register(t0);

SamplerState shadow_sampler : register(s1);
Texture2D  shadow_texture_0 : register(t1);
Texture2D  shadow_texture_1 : register(t2);
Texture2D  shadow_texture_2 : register(t3);
Texture2D  shadow_texture_3 : register(t4);

SamplerState entity_sampler : register(s5);
Texture2D    entity_texture : register(t5);

float get_shadow_factor(vs_out vs_data, float camera_z);

float4 p_shader(vs_out vs_data) : SV_Target
{
    // Setup
    vs_data.normal = normalize(vs_data.normal);
    for (int i = 0; i < SHADOW_CASCADE_COUNT; i++) {
        vs_data.light_coords[i] /= vs_data.light_coords[i].w;
        vs_data.light_coords[i].z = min(vs_data.light_coords[i].z, 1);
    }

    // Reflection calculations
    const float3 camera_pixel_direction = normalize(vs_data.world - camera_position);
    const float3 reflected_pixel_direction = reflect(camera_pixel_direction, vs_data.normal);
    const float3 reflected_sky_color = skybox_texture.Sample(skybox_sampler, reflected_pixel_direction);

    // Refraction calculations
    const float3 refracted_pixel_direction = refract(camera_pixel_direction, vs_data.normal, object_data.w);
    const float3 refracted_sky_color = skybox_texture.Sample(skybox_sampler, refracted_pixel_direction);

    // Shadow calculations
    const float camera_z = mul(float4(vs_data.world, 1), v_matrix).z;
    const float shadow_factor = get_shadow_factor(vs_data, camera_z);

    // Light calculations
    const float3 reflected_sun_direction = reflect(-light_data.xyz, vs_data.normal);
    const float specular_strength = dot(camera_pixel_direction, reflected_sun_direction);

    const float ambient_factor = light_data.w;
    const float diffuse_factor = max(dot(-light_data.xyz, vs_data.normal), 0);
    const float specular_factor = pow(max(specular_strength, 0), 64) * object_data.y;

    const float light_intensity = ambient_factor + (diffuse_factor + specular_factor) * shadow_factor;

    // Color calculations
    const float3 texture_color = entity_texture.Sample(entity_sampler, vs_data.textur);
    const float3 unlit_color = lerp(object_color, texture_color, object_data.x);
    const float3 lit_color = unlit_color * light_intensity;

    const float3 reflected_color = lerp(lit_color, reflected_sky_color, object_data.y);
    const float3 refracted_color = lerp(reflected_color, refracted_sky_color, object_data.z);

    // Final
    return float4(refracted_color, 1);
}

float get_shadow_factor(vs_out vs_data, float camera_z)
{
    if (camera_z < cascade_distances.x) {
        const float shadow_depth = shadow_texture_0.Sample(shadow_sampler, vs_data.light_coords[0].xy).r;
        return (shadow_depth < vs_data.light_coords[0].z) ? 0 : 1;
    }
    
    if (camera_z < cascade_distances.y) {
        const float shadow_depth = shadow_texture_1.Sample(shadow_sampler, vs_data.light_coords[1].xy).r;
        return (shadow_depth < vs_data.light_coords[1].z) ? 0 : 1;
    }
    
    if (camera_z < cascade_distances.z) {
        const float shadow_depth = shadow_texture_2.Sample(shadow_sampler, vs_data.light_coords[2].xy).r;
        return (shadow_depth < vs_data.light_coords[2].z) ? 0 : 1;
    }
    
    const float shadow_depth = shadow_texture_3.Sample(shadow_sampler, vs_data.light_coords[3].xy).r;
    return (shadow_depth < vs_data.light_coords[3].z) ? 0 : 1;
}
