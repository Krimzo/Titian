// Vertex shader
cbuffer VS_CB : register(b0)
{
    float4x4 W;
    float4x4 VP;
};

struct VS_OUT
{
    float4 position : SV_Position;
    float3 world : VS_World;
    float2 textur : VS_Texture;
    float3 normal : VS_Normal;
};

VS_OUT v_shader(const float3 position : KL_Position, const float2 textur : KL_Texture, const float3 normal : KL_Normal)
{
    VS_OUT data;
    data.world = mul(float4(position, 1.0f), W).xyz;
    data.position = mul(float4(data.world, 1.0f), VP);
    data.textur = textur;
    data.normal = mul(float4(normal, 0.0f), W).xyz;
    return data;
}

// Pixel shader
cbuffer PS_CB : register(b0)
{
    float4 OBJECT_COLOR; // (color.r, color.g, color.b, none)

    float4 OBJECT_MATERIAL; // (texture_blend, reflection_factor, refraction_factor, refraction_index)
    float4 OBJECT_TEXTURE_INFO; // (has_normal_map, has_roughness_map, none, none)
    
    float4 CAMERA_INFO; // (camera.x, camera.y, camera.z, skybox?)
    float4 CAMERA_BACKGROUND; // (color.r, color.g, color.b, color.a)

    float4 AMBIENT_LIGHT; // (color.r, color.g, color.b, intensity)
    float4 DIRECTIONAL_LIGHT; // (sun.x, sun.y, sun.z, sun_point_size)
};

SamplerState SKYBOX_SAMPLER : register(s0);
TextureCube  SKYBOX_TEXTURE : register(t0);

SamplerState ENTITY_SAMPLER : register(s1);
Texture2D ENTITY_TEXTURE : register(t1);
Texture2D NORMAL_TEXTURE : register(t2);
Texture2D ROUGHNESS_TEXTURE : register(t3);

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

float4 p_shader(VS_OUT data) : SV_Target
{
    // Setup
    data.normal = normalize(data.normal);
    const float3 pixel_normal = get_pixel_normal(data.world, data.normal, data.textur);
    const float pixel_reflectivity = get_pixel_reflectivity(OBJECT_MATERIAL.y, data.textur);

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
    const float3 diffuse_factor = max(dot(-DIRECTIONAL_LIGHT.xyz, pixel_normal), 0.0f);
    const float specular_factor = pow(max(specular_strength, 0.0f), 64.0f) * pixel_reflectivity;

    const float3 light_intensity = ambient_factor + diffuse_factor + specular_factor;

    // Color calculations
    const float3 texture_color = ENTITY_TEXTURE.Sample(ENTITY_SAMPLER, data.textur).xyz;
    const float3 unlit_color = lerp(OBJECT_COLOR.xyz, texture_color, OBJECT_MATERIAL.x);
    const float3 lit_color = unlit_color * light_intensity;

    const float3 reflected_color = lerp(lit_color, reflected_sky_color, pixel_reflectivity);
    const float3 refracted_color = lerp(reflected_color, refracted_sky_color, OBJECT_MATERIAL.z);

    // Final
    return float4(refracted_color, 1.0f);
}
