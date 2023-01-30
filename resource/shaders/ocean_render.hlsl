// Vertex shader
static const float pi = 3.14159265359f;

cbuffer VS_CB : register(b0)
{
    matrix   w_matrix;
    matrix  vp_matrix;
    float4  time_data; // (elapsed_time, delta_time, none, none)
    float4 ocean_data; // (amplitude, direction_frequency, direction_frequency, wave_shift)
};

struct vs_out
{
    float4 screen : SV_Position;
    float3 world  : VS_World;
    float3 normal : VS_Normal;
};

float3 alter_water_position(float3 position)
{
    position.y = ocean_data.x * sin(dot(ocean_data.yzw, float3(position.xz, time_data.x)));
    return position;
}

vs_out v_shader(float3 position : KL_Position)
{
    vs_out data;
    
    position = alter_water_position(position);
    
    data.world = mul(float4(position, 1), w_matrix).xyz;
    data.screen = mul(float4(data.world, 1), vp_matrix);
    
    return data;
}

// Geometry shader
[maxvertexcount(3)]
void g_shader(triangle vs_out input_data[3], inout TriangleStream<vs_out> output_stream)
{
    const float3 ab = input_data[1].world - input_data[0].world;
    const float3 ac = input_data[2].world - input_data[0].world;
    const float3 updated_normal = normalize(cross(ab, ac));
    
    for (int i = 0; i < 3; i++) {
        vs_out out_data = input_data[i];
        out_data.normal = updated_normal;
        output_stream.Append(out_data);
    }
}

// Pixel shader
cbuffer PS_CB : register(b0)
{
    float4  object_color;
    float4 sun_direction;
};

float4 p_shader(vs_out vs_data) : SV_Target
{
    // Light calculations
    vs_data.normal = normalize(vs_data.normal);
    const float light_intensity = max(dot(-sun_direction.xyz, vs_data.normal), 0);

    // Color calculations
    const float3 base_color = object_color.xyz;
    const float3 pixel_color = base_color * light_intensity;

    // Final
    return float4(pixel_color, 1);
}
