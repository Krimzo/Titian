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

/*
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

    float RECEIVES_SHADOWS;
    float2 SHADOW_MAP_SIZE;
    float2 SHADOW_MAP_TEXEL_SIZE;
    float4 SHADOW_CASCADES;
    float4x4 LIGHT_VPs[SHADOW_CASCADE_COUNT];

    float4x4 CUSTOM_DATA;
};

struct VS_OUT
{
    float4 screen : SV_Position;
    float3 world : VS_World;
    float2 textur : VS_Texture;
    float3 normal : VS_Normal;
    float4 light_coords[SHADOW_CASCADE_COUNT] : VS_Light;
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
*/