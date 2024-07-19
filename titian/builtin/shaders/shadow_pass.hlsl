static const uint MAX_BONE_COUNT = 100;

// Vertex shader
cbuffer VS_CB : register(b0)
{
	float4x4 WVP;
    float IS_SKELETAL;
}

StructuredBuffer<float4x4> BONE_MATRICES : register(t0);

float4 v_shader(float3 position : KL_Position, uint4 bone_indices : KL_BoneIndices, float4 bone_weights : KL_BoneWeights) : SV_Position
{
    if (IS_SKELETAL) {
        float4x4 bone_mat = BONE_MATRICES[bone_indices[0]] * bone_weights[0]
                          + BONE_MATRICES[bone_indices[1]] * bone_weights[1]
                          + BONE_MATRICES[bone_indices[2]] * bone_weights[2]
                          + BONE_MATRICES[bone_indices[3]] * bone_weights[3];
        position = mul(float4(position, 1.0f), bone_mat).xyz;
    }
	return mul(float4(position, 1.0f), WVP);
}

// Pixel shader
void p_shader()
{}
