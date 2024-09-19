struct VS_OUT
{
    float4 position : SV_Position;
    float3 normal : VS_Normal;
};

float4x4 W;
float4x4 WVP;
float4 OBJECT_COLOR;
float3 SUN_DIRECTION;
float IS_SKELETAL;

StructuredBuffer<float4x4> BONES_BUFFER : register(t0);

VS_OUT v_shader(float3 position : KL_Position, float3 normal : KL_Normal, uint4 bone_indices : KL_BoneIndices, float4 bone_weights : KL_BoneWeights)
{
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
    data.position = mul(float4(position, 1.0f), WVP);
    data.normal = mul(float4(normal, 0.0f), W).xyz;
    return data;
}

float4 p_shader(VS_OUT data) : SV_Target0
{
    float3 normal = normalize(data.normal);
    float diffuse = saturate(dot(normal, -SUN_DIRECTION));
    return OBJECT_COLOR * diffuse;
}
