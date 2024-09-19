#define SHADOW_CASCADE_COUNT 4

struct VS_OUT
{
    float4 screen : SV_Position;
    float3 world : VS_World;
    float2 textur : VS_Texture;
    float3 normal : VS_Normal;
    float4 light_coords[SHADOW_CASCADE_COUNT] : VS_Light;
};

struct PS_OUT
{
    float4 color : SV_Target0;
    float index : SV_Target1;
};

float4x4 W;
float4x4 V;
float4x4 VP;
float4x4 LIGHT_VPs[SHADOW_CASCADE_COUNT];
float4x4 CUSTOM_DATA;
float4 CAMERA_BACKGROUND;
float4 MATERIAL_COLOR;
float4 SHADOW_CASCADES;
float3 CAMERA_POSITION;
float CAMERA_HAS_SKYBOX;
float3 AMBIENT_COLOR;
float RECEIVES_SHADOWS;
float3 SUN_DIRECTION;
float SUN_POINT_SIZE;
float3 SUN_COLOR;
float ELAPSED_TIME;
float3 OBJECT_SCALE;
float DELTA_TIME;
float3 OBJECT_ROTATION;
float OBJECT_INDEX;
float3 OBJECT_POSITION;
float TEXTURE_BLEND;
float2 SHADOW_TEXTURE_SIZE;
float2 SHADOW_TEXTURE_TEXEL_SIZE;
float REFLECTIVITY_FACTOR;
float REFRACTION_INDEX;
float HAS_NORMAL_TEXTURE;
float HAS_ROUGHNESS_TEXTURE;
float IS_SKELETAL;

StructuredBuffer<float4x4> BONES_BUFFER : register(t0);
TextureCube SKYBOX_TEXTURE : register(t0);
Texture2D SHADOW_TEXTURE0 : register(t1);
Texture2D SHADOW_TEXTURE1 : register(t2);
Texture2D SHADOW_TEXTURE2 : register(t3);
Texture2D SHADOW_TEXTURE3 : register(t4);
Texture2D COLOR_TEXTURE : register(t5);
Texture2D NORMAL_TEXTURE : register(t6);
Texture2D ROUGHNESS_TEXTURE : register(t7);

SamplerState SKYBOX_SAMPLER : register(s0);
SamplerState SHADOW_SAMPLER : register(s1);
SamplerState MATERIAL_SAMPLER : register(s2);

float3 get_normal(float3 position, float3 normal, float2 coords)
{
    if (!HAS_NORMAL_TEXTURE)
        return normal;
    
    float3 Q1 = ddx(position);
    float3 Q2 = ddy(position);
    float2 st1 = ddx(coords);
    float2 st2 = ddy(coords);
    float3 T = normalize(Q1 * st2.x - Q2 * st1.x);
    float3 B = normalize(-Q1 * st2.y + Q2 * st1.y);
    float3x3 TBN = float3x3(T, B, normal);
    float3 result = normalize(NORMAL_TEXTURE.Sample(MATERIAL_SAMPLER, coords).xyz * 2.0f - 1.0f);
    return normalize(mul(result, TBN));
}

float get_reflectivity(float2 texture_coords)
{
    if (!HAS_ROUGHNESS_TEXTURE)
        return clamp(REFLECTIVITY_FACTOR, -1.0f, 1.0f);
    return 1.0f - ROUGHNESS_TEXTURE.Sample(MATERIAL_SAMPLER, texture_coords).r;
}

float get_pcf_shadow(Texture2D shadow_texture, float3 light_coords, int half_kernel_size)
{
    float shadow_factor = 0.0f;
    [unroll]
    for (int y = -half_kernel_size; y <= half_kernel_size; y++)
    {
        [unroll]
        for (int x = -half_kernel_size; x <= half_kernel_size; x++)
        {
            float2 kernel_coords = float2(x, y) + 0.25f;
            float2 altered_coords = light_coords.xy + kernel_coords * SHADOW_TEXTURE_TEXEL_SIZE;
            float shadow_depth = shadow_texture.Sample(SHADOW_SAMPLER, altered_coords).r;
            shadow_factor += (light_coords.z < shadow_depth) ? 0.0f : 1.0f;
        }
    }
    return shadow_factor / ((half_kernel_size * 2 + 1) * (half_kernel_size * 2 + 1));
}

float get_shadow(VS_OUT data, int half_kernel_size)
{
    if (!RECEIVES_SHADOWS)
        return 0.0f;
    
    float pcf_value = 0.0f;
    float camera_z = abs(mul(float4(data.world, 1.0f), V).z);
    if (camera_z < SHADOW_CASCADES.x)
    {
        pcf_value = get_pcf_shadow(SHADOW_TEXTURE0, data.light_coords[0].xyz, half_kernel_size);
    }
    else if (camera_z < SHADOW_CASCADES.y)
    {
        pcf_value = get_pcf_shadow(SHADOW_TEXTURE1, data.light_coords[1].xyz, half_kernel_size);
    }
    else if (camera_z < SHADOW_CASCADES.z)
    {
        pcf_value = get_pcf_shadow(SHADOW_TEXTURE2, data.light_coords[2].xyz, half_kernel_size);
    }
    else
    {
        pcf_value = get_pcf_shadow(SHADOW_TEXTURE3, data.light_coords[3].xyz, half_kernel_size);
    }
    return pcf_value;
}

#define _CUSTOM_VERTEX_SHADER_PLACEHOLDER

#define _CUSTOM_PIXEL_SHADER_PLACEHOLDER

VS_OUT v_shader(float3 position : KL_Position, float2 textur : KL_Texture, float3 normal : KL_Normal, int4 bone_indices : KL_BoneIndices, float4 bone_weights : KL_BoneWeights)
{
#ifdef _CUSTOM_VERTEX_SHADER
    _vertex_pre(position, textur, normal);
#endif
    
    if (IS_SKELETAL)
    {
        float4x4 bone_mat = BONES_BUFFER[bone_indices[0]] * bone_weights[0]
                          + BONES_BUFFER[bone_indices[1]] * bone_weights[1]
                          + BONES_BUFFER[bone_indices[2]] * bone_weights[2]
                          + BONES_BUFFER[bone_indices[3]] * bone_weights[3];
        position = mul(float4(position, 1.0f), bone_mat).xyz;
        normal = mul(float4(normal, 0.0f), bone_mat).xyz;
    }

    VS_OUT data;
    data.world = mul(float4(position, 1.0f), W).xyz;
    data.screen = mul(float4(data.world, 1.0f), VP);
    data.textur = textur;
    data.normal = mul(float4(normal, 0.0f), W).xyz;

    for (int i = 0; i < SHADOW_CASCADE_COUNT; i++)
    {
        data.light_coords[i] = mul(float4(position, 1.0f), mul(W, LIGHT_VPs[i]));
        data.light_coords[i].xy *= float2(0.5f, -0.5f);
        data.light_coords[i].xy += 0.5f;
    }
    
#ifdef _CUSTOM_VERTEX_SHADER
    _vertex_post(data);
#endif
    return data;
}

PS_OUT p_shader(VS_OUT data)
{
    float3 vertex_normal = normalize(data.normal);
    data.normal = get_normal(data.world, vertex_normal, data.textur);
    
    PS_OUT result;
    result.color = float4(0.0f, 0.0f, 0.0f, 1.0f);
    result.index = OBJECT_INDEX;
#ifdef _CUSTOM_PIXEL_SHADER
    if (_pixel_pre(data, result.color))
        return result;
#endif
    
    float pixel_reflectivity = get_reflectivity(data.textur);
    float3 pixel_dir = normalize(data.world - CAMERA_POSITION);
    
    float ambient_factor = saturate(dot(vertex_normal, data.normal));
    float3 ambient = AMBIENT_COLOR * ambient_factor;
    
    float diffuse_factor = saturate(dot(-SUN_DIRECTION, data.normal));
    float3 diffuse = SUN_COLOR * diffuse_factor;
    
    float3 refl_sun_dir = reflect(SUN_DIRECTION, data.normal);
    float specular_factor = saturate(dot(-pixel_dir, refl_sun_dir));
    float3 specular = pow(specular_factor, 1.0f / SUN_POINT_SIZE) * saturate(pixel_reflectivity);

    float3 shadow = get_shadow(data, 1);
    float3 light = ambient + (diffuse + specular) * (1.0f - shadow);

    float4 texture_color = COLOR_TEXTURE.Sample(MATERIAL_SAMPLER, data.textur);
    result.color = lerp(MATERIAL_COLOR, texture_color, TEXTURE_BLEND) * float4(light, 1.0f);
    
    if (pixel_reflectivity >= 0.0f)
    {
        float3 refl_pixel_dir = reflect(pixel_dir, data.normal);
        float4 refl_pixel_color = CAMERA_HAS_SKYBOX ? SKYBOX_TEXTURE.Sample(SKYBOX_SAMPLER, refl_pixel_dir) : CAMERA_BACKGROUND;
        result.color = lerp(result.color, refl_pixel_color, pixel_reflectivity);
    }
    else
    {
        float3 refr_pixel_dir = refract(pixel_dir, data.normal, 1.0f / REFRACTION_INDEX);
        float4 refr_pixel_color = CAMERA_HAS_SKYBOX ? SKYBOX_TEXTURE.Sample(SKYBOX_SAMPLER, refr_pixel_dir) : CAMERA_BACKGROUND;
        result.color = lerp(result.color, refr_pixel_color, -pixel_reflectivity);
    }
    
#ifdef _CUSTOM_PIXEL_SHADER
    _pixel_post(data, result.color);
#endif
    return result;
}
