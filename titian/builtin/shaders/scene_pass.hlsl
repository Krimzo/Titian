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
        data.light_coords[i].xy += float2(0.5f, 0.5f) * data.light_coords[i].w;
    }
    return data;
}

// Pixel shader
cbuffer PS_CB : register(b0)
{
    float4 OBJECT_COLOR; // (color.r, color.g, color.b, none)
    float4 OBJECT_INDEX; // (index, index, index, index)

    float4 OBJECT_MATERIAL; // (texture_blend, reflection_factor, refraction_factor, refraction_index)
    float4 OBJECT_TEXTURE_INFO; // (has_normal_map, has_roughness_map, none, none)
    
    float4 CAMERA_INFO; // (camera.x, camera.y, camera.z, skybox?)
    float4 CAMERA_BACKGROUND; // (color.r, color.g, color.b, color.a)
    float4x4 V; // View matrix

    float4 AMBIENT_LIGHT; // (color.r, color.g, color.b, intensity)
    float4 DIRECTIONAL_LIGHT; // (dir.x, dir.y, dir.z, sun_point_size)
    float4 DIRECTIONAL_LIGHT_COLOR; // (color.r, color.g, color.b, none)

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

VS_OUT compute_light_transforms(VS_OUT data)
{
    [unroll]
    for (int i = 0; i < SHADOW_CASCADE_COUNT; i++) {
        data.light_coords[i] /= data.light_coords[i].w;
        data.light_coords[i].z = min(data.light_coords[i].z, 1);
    }
    return data;
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

PS_OUT p_shader(VS_OUT data)
{
    // Setup
    data.normal = normalize(data.normal);
    const float3 pixel_normal = get_pixel_normal(data.world, data.normal, data.textur);
    const float pixel_reflectivity = get_pixel_reflectivity(OBJECT_MATERIAL.y, data.textur);
    data = compute_light_transforms(data);

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
    float3 diffuse_factor = DIRECTIONAL_LIGHT_COLOR.xyz;

    if (ambient_diffuse_factor > 0.0f) {
        const float camera_z = abs(mul(float4(data.world, 1.0f), V).z);
        const float shadow_factor = get_shadow_factor(data, camera_z, 1.0f);
        diffuse_factor *= directional_diffuse_factor * max(ambient_factor, shadow_factor);
    }
    else {
        diffuse_factor *= directional_diffuse_factor * ambient_factor;
    }
    const float3 light_intensity = ambient_factor + diffuse_factor + specular_factor;

    // Color calculations
    const float3 texture_color = ENTITY_TEXTURE.Sample(ENTITY_SAMPLER, data.textur).xyz;
    const float3 unlit_color = lerp(OBJECT_COLOR.xyz, texture_color, OBJECT_MATERIAL.x);
    const float3 lit_color = unlit_color * light_intensity;

    const float3 reflected_color = lerp(lit_color, reflected_sky_color, pixel_reflectivity);
    const float3 refracted_color = lerp(reflected_color, refracted_sky_color, OBJECT_MATERIAL.z);

    // Final
    PS_OUT result;
    result.color = float4(refracted_color, 1.0f);
    result.index = OBJECT_INDEX.r;
    return result;
}
