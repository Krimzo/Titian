float REC_ITERATIONS;

RWTexture2D<float4> TARGET : register(u0);
RWTexture2D<float4> TARGET_COPY : register(u1);

[numthreads(32, 32, 1)]
void c_shader(uint3 id : SV_DispatchThreadID)
{
	uint2 resolution;
	TARGET.GetDimensions(resolution.x, resolution.y);
	if (id.x >= resolution.x || id.y >= resolution.y)
		return;
	
	TARGET_COPY[id.xy] = TARGET[id.xy] * REC_ITERATIONS;
}
