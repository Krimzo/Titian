#define _CUSTOM_VERTEX_SHADER
void _vertex_pre(inout float3 position, inout float2 textur, inout float3 normal)
{
}

void _vertex_post(inout VS_OUT data)
{
}

#define _CUSTOM_PIXEL_SHADER
bool _pixel_pre(inout VS_OUT data, inout float4 color)
{
    return false;
}

void _pixel_post(inout VS_OUT data, inout float4 color)
{
}

#if false
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
#endif
