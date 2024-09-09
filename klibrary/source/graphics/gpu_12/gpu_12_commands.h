#pragma once

#include "math/math.h"


namespace kl {
	struct GPU12Commands
	{
		dx12::CommandAllocator allocator{};
		dx12::CommandList list{};

		GPU12Commands();
		GPU12Commands(const dx12::CommandAllocator& allocator);
		GPU12Commands(const dx12::CommandAllocator& allocator, const dx12::CommandList& list);

		void reset() const;
		void close() const;

		void transition_resource(const dx12::Resource& resource, D3D12_RESOURCE_STATES from, D3D12_RESOURCE_STATES to) const;

		void clear_target_view(const dx12::DescriptorHandle& handle, const Float4& color) const;
		void copy(dx12::Resource& destination, const dx12::Resource& source) const;

		void set_root_signature(const dx12::RootSignature& root_signature) const;
		void set_pipeline_state(const dx12::PipelineState& pipeline_state) const;

		void set_primitive_topology(D3D_PRIMITIVE_TOPOLOGY topology) const;
		void set_vertex_buffer(const dx12::VertexBuffer& vertex_buffer) const;

		void set_scissors(const dx12::Scissors& scissors) const;
		void set_viewport(const dx12::Viewport& viewport) const;

		void set_render_target(const dx12::DescriptorHandle* render_target, const dx12::DescriptorHandle* depth_target) const;
		void draw(UINT vertex_count, UINT instance_count = 1) const;

		void update_tlas(const dx12::AccelerationStructure& tlas, const dx12::AccelerationStructure& tlas_update_scratch, const dx12::Resource& instances) const;
		void dispatch_rays(D3D12_GPU_VIRTUAL_ADDRESS shader_address, UINT width, UINT height) const;
	};
}
