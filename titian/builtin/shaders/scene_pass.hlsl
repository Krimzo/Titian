// Constants
#define SHADOW_CASCADE_COUNT 4

// Vertex shader
cbuffer VS_CB : register(b0)
{
    float4x4 W;
    float4x4 VP;
    float4x4 LIGHT_VPs[SHADOW_CASCADE_COUNT];
};

struct VS_OUT
{
    float4 screen : SV_Position;
    float3 world : VS_World;
    float2 textur : VS_Texture;
    float3 normal : VS_Normal;
    float4 light_coords[SHADOW_CASCADE_COUNT] : VS_Light;
};

#define _ALTER_VERTEX_NOT_DECLARED
void _alter_vertex(inout VS_OUT data) {}

VS_OUT v_shader(const float3 position : KL_Position, const float2 textur : KL_Texture, const float3 normal : KL_Normal)
{
    VS_OUT data;

    data.world = mul(float4(position, 1.0f), W).xyz;
    data.screen = mul(float4(data.world, 1.0f), VP);
    data.textur = textur;
    data.normal = mul(float4(normal, 0.0f), W).xyz;

    for (int i = 0; i < SHADOW_CASCADE_COUNT; i++) {
        data.light_coords[i] = mul(float4(position, 1.0f), mul(W, LIGHT_VPs[i]));
        data.light_coords[i].xy *= float2(0.5f, -0.5f);
        data.light_coords[i].xy += 0.5f;
    }
    
#ifndef _ALTER_VERTEX_NOT_DECLARED
    _alter_vertex(data);
#endif
    
    return data;
}

// Pixel shader
cbuffer PS_CB : register(b0)
{
    float3 OBJECT_COLOR; // (color.r, color.g, color.b)
    float OBJECT_INDEX; // (index)

    float4 OBJECT_MATERIAL; // (texture_blend, reflection_factor, refraction_factor, refraction_index)
    float2 OBJECT_TEXTURE_INFO; // (has_normal_map, has_roughness_map)
    
    float4 CAMERA_INFO; // (camera.x, camera.y, camera.z, skybox?)
    float4 CAMERA_BACKGROUND; // (color.r, color.g, color.b, color.a)
    float4x4 V; // View matrix

    float4 AMBIENT_LIGHT; // (color.r, color.g, color.b, intensity)
    float4 DIRECTIONAL_LIGHT; // (dir.x, dir.y, dir.z, sun_point_size)
    float3 DIRECTIONAL_LIGHT_COLOR; // (color.r, color.g, color.b)

    float HAS_SHADOWS; // (has_shadows)
    float4 SHADOW_MAP_INFO; // (width, height, texel_width, texel_size)
    float4 CASCADE_DISTANCES; // (cascade_0_far, cascade_1_far, cascade_2_far, cascade_3_far)
};

struct PS_OUT
{
    float4 color : SV_Target0;
    float index : SV_Target1;
};

SamplerState SKYBOX_SAMPLER : register(s0);
TextureCube SKYBOX_TEXTURE : register(t0);

SamplerState SHADOW_SAMPLER : register(s1);
Texture2D SHADOW_TEXTURE_0 : register(t1);
Texture2D SHADOW_TEXTURE_1 : register(t2);
Texture2D SHADOW_TEXTURE_2 : register(t3);
Texture2D SHADOW_TEXTURE_3 : register(t4);

SamplerState ENTITY_SAMPLER : register(s2);
Texture2D ENTITY_TEXTURE : register(t5);
Texture2D NORMAL_TEXTURE : register(t6);
Texture2D ROUGHNESS_TEXTURE : register(t7);

float3 get_pixel_normal(const float3 world_position, const float3 interpolated_normal, const float2 texture_coords)
{
    if (!OBJECT_TEXTURE_INFO.x) {
        return interpolated_normal;
    }

    const float3 Q1 = ddx(world_position);
    const float3 Q2 = ddy(world_position);
    const float2 st1 = ddx(texture_coords);
    const float2 st2 = ddy(texture_coords);
    const float3 T = normalize(Q1 * st2.x - Q2 * st1.x);
    const float3 B = normalize(-Q1 * st2.y + Q2 * st1.y);
    const float3x3 TBN = float3x3(T, B, interpolated_normal);
    const float3 result_normal = normalize(NORMAL_TEXTURE.Sample(ENTITY_SAMPLER, texture_coords).xyz * 2.0f - 1.0f);
    return normalize(mul(result_normal, TBN));
}

float get_pixel_reflectivity(const float reflectivity, const float2 texture_coords)
{
    if (!OBJECT_TEXTURE_INFO.y) {
        return reflectivity;
    }
    const float roughness = ROUGHNESS_TEXTURE.Sample(ENTITY_SAMPLER, texture_coords).r;
    return 1.0f - roughness;
}

float get_pcf_shadow(const Texture2D shadow_map, const float3 light_coords, const int half_kernel_size)
{
    static const float2 adder = 0.25f;

    float shadow_factor = 0.0f;
    float sample_counter = 0.0f;

    [unroll]
    for (int y = -half_kernel_size; y <= half_kernel_size; y++) {
        [unroll]
        for (int x = -half_kernel_size; x <= half_kernel_size; x++) {
            const float2 kernel_coords = float2(x, y) + adder;
            const float2 altered_coords = light_coords.xy + kernel_coords * SHADOW_MAP_INFO.zw;
            const float depth = shadow_map.Sample(SHADOW_SAMPLER, altered_coords).r;
            
            shadow_factor += (depth < light_coords.z) ? 0.0f : 1.0f;
            sample_counter += 1.0f;
        }
    }

    return (shadow_factor / sample_counter);
}

float get_shadow_factor(const VS_OUT data, const float camera_z, const int half_kernel_size)
{
    float pcf_value = 0.0f;
    if (camera_z < CASCADE_DISTANCES.x) {
        pcf_value = get_pcf_shadow(SHADOW_TEXTURE_0, data.light_coords[0].xyz, half_kernel_size);
    }
    else if (camera_z < CASCADE_DISTANCES.y) {
        pcf_value = get_pcf_shadow(SHADOW_TEXTURE_1, data.light_coords[1].xyz, half_kernel_size);
    }
    else if (camera_z < CASCADE_DISTANCES.z) {
        pcf_value = get_pcf_shadow(SHADOW_TEXTURE_2, data.light_coords[2].xyz, half_kernel_size);
    }
    else {
        pcf_value = get_pcf_shadow(SHADOW_TEXTURE_3, data.light_coords[3].xyz, half_kernel_size);
    }
    return pcf_value;
}

#define _ALTER_UNLIT_NOT_DECLARED
bool _alter_unlit(inout VS_OUT data, inout float3 result_color) { return false; }

#define _ALTER_LIT_NOT_DECLARED
void _alter_lit(inout float3 color) {}

PS_OUT p_shader(VS_OUT data)
{
    // Setup
    data.normal = normalize(data.normal);
    const float3 pixel_normal = get_pixel_normal(data.world, data.normal, data.textur);
    const float pixel_reflectivity = get_pixel_reflectivity(OBJECT_MATERIAL.y, data.textur);
    
#ifndef _ALTER_UNLIT_NOT_DECLARED
    // Unlit
    float3 _result_color = 0.0f;
    if (_alter_unlit(data, _result_color)) {
        PS_OUT result;
        result.color = float4(_result_color, 1.0f);
        result.index = OBJECT_INDEX;
        return result;
    }
#endif

    // Reflection calculations
    const float3 camera_pixel_direction = normalize(data.world - CAMERA_INFO.xyz);
    const float3 reflected_pixel_direction = reflect(camera_pixel_direction, pixel_normal);
    const float3 reflected_sky_color = CAMERA_INFO.w ? SKYBOX_TEXTURE.Sample(SKYBOX_SAMPLER, reflected_pixel_direction).xyz : CAMERA_BACKGROUND.xyz;
    
    // Refraction calculations
    const float3 refracted_pixel_direction = refract(camera_pixel_direction, pixel_normal, OBJECT_MATERIAL.w);
    const float3 refracted_sky_color = CAMERA_INFO.w ? SKYBOX_TEXTURE.Sample(SKYBOX_SAMPLER, refracted_pixel_direction).xyz : CAMERA_BACKGROUND.xyz;

    // Light calculations
    const float3 reflected_sun_direction = reflect(-DIRECTIONAL_LIGHT.xyz, pixel_normal);
    const float specular_strength = dot(camera_pixel_direction, reflected_sun_direction);

    const float3 ambient_factor = AMBIENT_LIGHT.xyz * AMBIENT_LIGHT.w;
    const float specular_factor = pow(max(specular_strength, 0.0f), 64.0f) * pixel_reflectivity;

    const float ambient_diffuse_factor = dot(-DIRECTIONAL_LIGHT.xyz, data.normal);
    const float directional_diffuse_factor = dot(-DIRECTIONAL_LIGHT.xyz, pixel_normal);
    float3 diffuse_factor = DIRECTIONAL_LIGHT_COLOR;

    if (HAS_SHADOWS) {
        if (ambient_diffuse_factor > 0.0f) {
            const float camera_z = abs(mul(float4(data.world, 1.0f), V).z);
            const float shadow_factor = get_shadow_factor(data, camera_z, 1.0f);
            diffuse_factor *= directional_diffuse_factor * max(ambient_factor, shadow_factor);
        }
        else {
            diffuse_factor *= directional_diffuse_factor * ambient_factor;
        }
    }
    else {
        diffuse_factor *= directional_diffuse_factor;
    }
    const float3 light_intensity = ambient_factor + diffuse_factor + specular_factor;

    // Color calculations
    const float3 texture_color = ENTITY_TEXTURE.Sample(ENTITY_SAMPLER, data.textur).xyz;
    const float3 unlit_color = lerp(OBJECT_COLOR, texture_color, OBJECT_MATERIAL.x);
    const float3 lit_color = unlit_color * light_intensity;

    const float3 reflected_color = lerp(lit_color, reflected_sky_color, pixel_reflectivity);
    float3 refracted_color = lerp(reflected_color, refracted_sky_color, OBJECT_MATERIAL.z);

#ifndef _ALTER_LIT_NOT_DECLARED
    _alter_lit(refracted_color);
#endif

    // Final
    PS_OUT result;
    result.color = float4(refracted_color, 1.0f);
    result.index = OBJECT_INDEX;
    return result;
}
