#pragma once

#include "math/math.h"


namespace kl
{
struct GPU12Commands
{
    dx12::CommandAllocator allocator{};
    dx12::CommandList list{};

    GPU12Commands();
    GPU12Commands( dx12::CommandAllocator const& allocator );
    GPU12Commands( dx12::CommandAllocator const& allocator, dx12::CommandList const& list );

    void reset() const;
    void close() const;

    void transition_resource( dx12::Resource const& resource, D3D12_RESOURCE_STATES from, D3D12_RESOURCE_STATES to ) const;

    void clear_target_view( dx12::DescriptorHandle const& handle, Float4 const& color ) const;
    void copy( dx12::Resource const& destination, dx12::Resource const& source ) const;

    void set_root_signature( dx12::RootSignature const& root_signature ) const;
    void set_pipeline_state( dx12::PipelineState const& pipeline_state ) const;

    void set_primitive_topology( D3D_PRIMITIVE_TOPOLOGY topology ) const;
    void set_vertex_buffer( dx12::VertexBuffer const& vertex_buffer ) const;

    void set_scissors( dx12::Scissors const& scissors ) const;
    void set_viewport( dx12::Viewport const& viewport ) const;

    void set_render_target( dx12::DescriptorHandle const* render_target, dx12::DescriptorHandle const* depth_target ) const;
    void draw( UINT vertex_count, UINT instance_count = 1 ) const;

    void update_tlas( dx12::AccelerationStructure const& tlas, dx12::AccelerationStructure const& tlas_update_scratch, dx12::Resource const& instances ) const;
    void dispatch_rays( D3D12_GPU_VIRTUAL_ADDRESS shader_address, UINT width, UINT height ) const;
};
}
