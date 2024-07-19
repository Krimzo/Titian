static const uint MAX_BONE_COUNT = 100;

// Vertex shader
cbuffer VS_CB : register(b0)
{
    float4x4 W;
	float4x4 WVP;
    
    float4x4 BONE_MATRICES[MAX_BONE_COUNT];
    float IS_SKELETAL;
}

struct VS_OUT
{
    float4 position : SV_Position;
    float3 normal : VS_Normal;
};

VS_OUT v_shader(float3 position : KL_Position, float3 normal : KL_Normal, uint4 bone_indices : KL_BoneIndices, float4 bone_weights : KL_BoneWeights)
{
    if (IS_SKELETAL) {
        float4x4 bone_mat = BONE_MATRICES[bone_indices[0]] * bone_weights[0]
                          + BONE_MATRICES[bone_indices[1]] * bone_weights[1]
                          + BONE_MATRICES[bone_indices[2]] * bone_weights[2]
                          + BONE_MATRICES[bone_indices[3]] * bone_weights[3];
        position = mul(float4(position, 1.0f), bone_mat).xyz;
        normal = mul(float4(normal, 0.0f), bone_mat).xyz;
    }
    
    VS_OUT data;
    data.position = mul(float4(position, 1.0f), WVP);
    data.normal = mul(float4(normal, 0.0f), W).xyz;
    return data;
}

// Pixel shader
cbuffer PS_CB : register(b0)
{
    float4 OBJECT_COLOR;
    float3 SUN_DIRECTION;
}

float4 p_shader(const VS_OUT data) : SV_Target0
{
    const float3 normal = normalize(data.normal);
    const float diffuse = max(dot(normal, -SUN_DIRECTION), 0.0f);
    return OBJECT_COLOR * diffuse;
}
