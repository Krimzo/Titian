// Global
static const uint SHADOW_CASCADE_COUNT = 4;

cbuffer GLOBAL_CB : register(b0)
{
    float ELAPSED_TIME;
    float DELTA_TIME;
    
    float3 CAMERA_POSITION;
    float CAMERA_HAS_SKYBOX;
    float4 CAMERA_BACKGROUND;
    
    float3 AMBIENT_COLOR;
    float AMBIENT_INTENSITY;
    
    float3 SUN_DIRECTION;
    float SUN_POINT_SIZE;
    float3 SUN_COLOR;
    
    float OBJECT_INDEX;
    float3 OBJECT_SCALE;
    float3 OBJECT_ROTATION;
    float3 OBJECT_POSITION;

    float4 MATERIAL_COLOR;
    float TEXTURE_BLEND;
    
    float REFLECTION_FACTOR;
    float REFRACTION_FACTOR;
    float REFRACTION_INDEX;
    
    float HAS_NORMAL_MAP;
    float HAS_ROUGHNESS_MAP;
    
    float4x4 W;
    float4x4 V;
    float4x4 VP;
    
    float IS_SKELETAL;

    float RECEIVES_SHADOWS;
    float2 SHADOW_MAP_SIZE;
    float2 SHADOW_MAP_TEXEL_SIZE;
    float4 SHADOW_CASCADES;
    float4x4 LIGHT_VPs[SHADOW_CASCADE_COUNT];
    
    float4x4 CUSTOM_DATA;
};

// Vertex shader
struct VS_OUT
{
    float4 screen : SV_Position;
    float3 world : VS_World;
    float2 textur : VS_Texture;
    float3 normal : VS_Normal;
    float4 light_coords[SHADOW_CASCADE_COUNT] : VS_Light;
};

StructuredBuffer<float4x4> BONE_MATRICES : register(t0);

#define _CUSTOM_VERTEX_SHADER_PLACEHOLDER

VS_OUT v_shader(float3 position : KL_Position, float2 textur : KL_Texture, float3 normal : KL_Normal, uint4 bone_indices : KL_BoneIndices, float4 bone_weights : KL_BoneWeights)
{
#ifdef _CUSTOM_VERTEX_SHADER
    _vertex_pre(position, textur, normal);
#endif
    
    if (IS_SKELETAL) {
        float4x4 bone_mat = BONE_MATRICES[bone_indices[0]] * bone_weights[0]
                          + BONE_MATRICES[bone_indices[1]] * bone_weights[1]
                          + BONE_MATRICES[bone_indices[2]] * bone_weights[2]
                          + BONE_MATRICES[bone_indices[3]] * bone_weights[3];
        position = mul(float4(position, 1.0f), bone_mat).xyz;
        normal = mul(float4(normal, 0.0f), bone_mat).xyz;
    }

    VS_OUT data;
    data.world = mul(float4(position, 1.0f), W).xyz;
    data.screen = mul(float4(data.world, 1.0f), VP);
    data.textur = textur;
    data.normal = mul(float4(normal, 0.0f), W).xyz;

    for (uint i = 0; i < SHADOW_CASCADE_COUNT; i++) {
        data.light_coords[i] = mul(float4(position, 1.0f), mul(W, LIGHT_VPs[i]));
        data.light_coords[i].xy *= float2(0.5f, -0.5f);
        data.light_coords[i].xy += 0.5f;
    }
    
#ifdef _CUSTOM_VERTEX_SHADER
    _vertex_post(data);
#endif
    
    return data;
}

// Pixel shader
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

SamplerState MATERIAL_SAMPLER : register(s2);
Texture2D COLOR_MAP : register(t5);
Texture2D NORMAL_MAP : register(t6);
Texture2D ROUGHNESS_MAP : register(t7);

float3 get_pixel_normal(const float3 world_position, const float3 interpolated_normal, const float2 texture_coords)
{
    if (!HAS_NORMAL_MAP) {
        return interpolated_normal;
    }

    const float3 Q1 = ddx(world_position);
    const float3 Q2 = ddy(world_position);
    const float2 st1 = ddx(texture_coords);
    const float2 st2 = ddy(texture_coords);
    const float3 T = normalize(Q1 * st2.x - Q2 * st1.x);
    const float3 B = normalize(-Q1 * st2.y + Q2 * st1.y);
    const float3x3 TBN = float3x3(T, B, interpolated_normal);
    const float3 result_normal = normalize(NORMAL_MAP.Sample(MATERIAL_SAMPLER, texture_coords).xyz * 2.0f - 1.0f);
    return normalize(mul(result_normal, TBN));
}

float get_pixel_reflectivity(const float reflectivity, const float2 texture_coords)
{
    if (!HAS_ROUGHNESS_MAP) {
        return reflectivity;
    }
    const float roughness = ROUGHNESS_MAP.Sample(MATERIAL_SAMPLER, texture_coords).r;
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
            const float2 altered_coords = light_coords.xy + kernel_coords * SHADOW_MAP_TEXEL_SIZE;
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
    if (camera_z < SHADOW_CASCADES.x) {
        pcf_value = get_pcf_shadow(SHADOW_TEXTURE_0, data.light_coords[0].xyz, half_kernel_size);
    }
    else if (camera_z < SHADOW_CASCADES.y) {
        pcf_value = get_pcf_shadow(SHADOW_TEXTURE_1, data.light_coords[1].xyz, half_kernel_size);
    }
    else if (camera_z < SHADOW_CASCADES.z) {
        pcf_value = get_pcf_shadow(SHADOW_TEXTURE_2, data.light_coords[2].xyz, half_kernel_size);
    }
    else {
        pcf_value = get_pcf_shadow(SHADOW_TEXTURE_3, data.light_coords[3].xyz, half_kernel_size);
    }
    return pcf_value;
}

#define _CUSTOM_PIXEL_SHADER_PLACEHOLDER

PS_OUT p_shader(VS_OUT data)
{
    // Setup
    data.normal = normalize(data.normal);
    const float3 pixel_normal = get_pixel_normal(data.world, data.normal, data.textur);
    const float pixel_reflectivity = get_pixel_reflectivity(REFLECTION_FACTOR, data.textur);

    PS_OUT result;
    result.color = float4(0.0f, 0.0f, 0.0f, 1.0f);
    result.index = OBJECT_INDEX;
    
#ifdef _CUSTOM_PIXEL_SHADER
    if (_pixel_pre(data, result.color)) {
        return result;
    }
#endif

    // Reflection calculations
    const float3 camera_pixel_direction = normalize(data.world - CAMERA_POSITION);
    const float3 reflected_pixel_direction = reflect(camera_pixel_direction, pixel_normal);
    const float4 reflected_sky_color = CAMERA_HAS_SKYBOX ? SKYBOX_TEXTURE.Sample(SKYBOX_SAMPLER, reflected_pixel_direction) : CAMERA_BACKGROUND;
    
    // Refraction calculations
    const float3 refracted_pixel_direction = refract(camera_pixel_direction, pixel_normal, REFRACTION_INDEX);
    const float4 refracted_sky_color = CAMERA_HAS_SKYBOX ? SKYBOX_TEXTURE.Sample(SKYBOX_SAMPLER, refracted_pixel_direction) : CAMERA_BACKGROUND;

    // Light calculations
    const float3 reflected_sun_direction = reflect(-SUN_DIRECTION, pixel_normal);
    const float specular_strength = dot(camera_pixel_direction, reflected_sun_direction);

    const float3 ambient_factor = AMBIENT_COLOR * AMBIENT_INTENSITY;
    const float specular_factor = pow(max(specular_strength, 0.0f), 64.0f) * pixel_reflectivity;

    const float ambient_diffuse_factor = dot(-SUN_DIRECTION, data.normal);
    const float directional_diffuse_factor = dot(-SUN_DIRECTION, pixel_normal);
    float3 diffuse_factor = SUN_COLOR;

    if (RECEIVES_SHADOWS) {
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
    const float4 texture_color = COLOR_MAP.Sample(MATERIAL_SAMPLER, data.textur);
    const float4 unlit_color = lerp(MATERIAL_COLOR, texture_color, TEXTURE_BLEND);
    const float4 lit_color = unlit_color * float4(light_intensity, 1.0f);
    
    const float4 reflected_color = lerp(lit_color, reflected_sky_color, pixel_reflectivity);
    const float4 refracted_color = lerp(reflected_color, refracted_sky_color, REFRACTION_FACTOR);
    
    result.color = refracted_color;
    
#ifdef _CUSTOM_PIXEL_SHADER
    _pixel_post(data, result.color);
#endif

    return result;
}
