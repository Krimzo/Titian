#include "klibrary.h"


kl::GPU12Commands::GPU12Commands()
{}

kl::GPU12Commands::GPU12Commands(const dx12::CommandAllocator& allocator)
	: allocator(allocator)
{}

kl::GPU12Commands::GPU12Commands(const dx12::CommandAllocator& allocator, const dx12::CommandList& list)
	: allocator(allocator), list(list)
{}

void kl::GPU12Commands::reset() const
{
	allocator->Reset() >> verify_result;
	list->Reset(allocator.get(), nullptr) >> verify_result;
}

void kl::GPU12Commands::close() const
{
	list->Close() >> verify_result;
}

void kl::GPU12Commands::transition_resource(const dx12::Resource& resource, const D3D12_RESOURCE_STATES from, const D3D12_RESOURCE_STATES to) const
{
	const D3D12_RESOURCE_BARRIER barrier{
		.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
		.Transition = { 
			.pResource = resource.get(),
			.StateBefore = from,
			.StateAfter = to,
		},
	};
	list->ResourceBarrier(1, &barrier);
}

void kl::GPU12Commands::clear_target_view(const dx12::DescriptorHandle& handle, const Float4& color) const
{
	list->ClearRenderTargetView(handle, &color.x, 0, nullptr);
}

void kl::GPU12Commands::copy(dx12::Resource& destination, const dx12::Resource& source) const
{
	list->CopyResource(destination.get(), source.get());
}

void kl::GPU12Commands::set_root_signature(const dx12::RootSignature& root_signature) const
{
	list->SetGraphicsRootSignature(root_signature.get());
}

void kl::GPU12Commands::set_pipeline_state(const dx12::PipelineState& pipeline_state) const
{
	list->SetPipelineState(pipeline_state.get());
}

void kl::GPU12Commands::set_primitive_topology(const D3D_PRIMITIVE_TOPOLOGY topology) const
{
	list->IASetPrimitiveTopology(topology);
}

void kl::GPU12Commands::set_vertex_buffer(const dx12::VertexBuffer& vertex_buffer) const
{
	list->IASetVertexBuffers(0, 1, &vertex_buffer);
}

void kl::GPU12Commands::set_scissors(const dx12::Scissors& scissors) const
{
	list->RSSetScissorRects(1, &scissors);
}

void kl::GPU12Commands::set_viewport(const dx12::Viewport& viewport) const
{
	list->RSSetViewports(1, &viewport);
}

void kl::GPU12Commands::set_render_target(const dx12::DescriptorHandle* render_target, const dx12::DescriptorHandle* depth_target) const
{
	list->OMSetRenderTargets(1, render_target, true, depth_target);
}

void kl::GPU12Commands::draw(const UINT vertex_count, const UINT instance_count) const
{
	list->DrawInstanced(vertex_count, instance_count, 0, 0);
}

void kl::GPU12Commands::update_tlas(const dx12::AccelerationStructure& tlas, const dx12::AccelerationStructure& tlas_update_scratch, const dx12::Resource& instances) const
{
	const UINT instance_count = UINT(instances->GetDesc().Width / sizeof(D3D12_RAYTRACING_INSTANCE_DESC));
	const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC acceleration_descriptor{
		.DestAccelerationStructureData = tlas->GetGPUVirtualAddress(),
		.Inputs{
			.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL,
			.Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PERFORM_UPDATE,
			.NumDescs = instance_count,
			.DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY,
			.InstanceDescs = instances->GetGPUVirtualAddress(),
		},
		.SourceAccelerationStructureData = tlas->GetGPUVirtualAddress(),
		.ScratchAccelerationStructureData = tlas_update_scratch->GetGPUVirtualAddress(),
	};
	list->BuildRaytracingAccelerationStructure(&acceleration_descriptor, 0, nullptr);
}

void kl::GPU12Commands::dispatch_rays(const D3D12_GPU_VIRTUAL_ADDRESS shader_address, const UINT width, const UINT height) const
{
	const D3D12_DISPATCH_RAYS_DESC dispatch_descriptor{
			.RayGenerationShaderRecord{
				.StartAddress = shader_address,
				.SizeInBytes = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES,
			},
			.MissShaderTable{
				.StartAddress = shader_address + D3D12_RAYTRACING_SHADER_TABLE_BYTE_ALIGNMENT,
				.SizeInBytes = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES,
			},
			.HitGroupTable{
				.StartAddress = shader_address + D3D12_RAYTRACING_SHADER_TABLE_BYTE_ALIGNMENT * 2,
				.SizeInBytes = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES,
			},
			.Width = width,
			.Height = height,
			.Depth = 1,
	};
	list->DispatchRays(&dispatch_descriptor);
}
