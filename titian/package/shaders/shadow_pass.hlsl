float4x4 WVP;
float IS_SKELETAL;

StructuredBuffer<float4x4> BONES_BUFFER : register(t0);

float4 v_shader(float3 position : KL_Position, uint4 bone_indices : KL_BoneIndices, float4 bone_weights : KL_BoneWeights) : SV_Position
{
    if (IS_SKELETAL)
    {
        float4x4 bone_mat = BONES_BUFFER[bone_indices[0]] * bone_weights[0]
                          + BONES_BUFFER[bone_indices[1]] * bone_weights[1]
                          + BONES_BUFFER[bone_indices[2]] * bone_weights[2]
                          + BONES_BUFFER[bone_indices[3]] * bone_weights[3];
        position = mul(float4(position, 1.0f), bone_mat).xyz;
    }
	return mul(float4(position, 1.0f), WVP);
}

void p_shader()
{}
