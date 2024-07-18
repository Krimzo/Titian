static const uint MAX_BONE_COUNT = 100;

// Vertex shader
cbuffer VS_CB : register(b0)
{
	float4x4 WVP;
	
    float4x4 BONE_MATRICES[MAX_BONE_COUNT];
    float IS_SKELETAL;
}

float4 v_shader(float3 position : KL_Position, uint bone_indices : KL_BoneIndices, float4 bone_weights : KL_BoneWeights) : SV_Position
{
    if (IS_SKELETAL) {
        float4x4 bone_mat =
            BONE_MATRICES[(bone_indices >> 24) & 0xFF] * bone_weights.x +
            BONE_MATRICES[(bone_indices >> 16) & 0xFF] * bone_weights.y +
            BONE_MATRICES[(bone_indices >> 8) & 0xFF] * bone_weights.z +
            BONE_MATRICES[(bone_indices >> 0) & 0xFF] * bone_weights.w;
        position = mul(float4(position, 1.0f), bone_mat).xyz;
    }
	return mul(float4(position, 1.0f), WVP);
}

// Pixel shader
void p_shader()
{}
