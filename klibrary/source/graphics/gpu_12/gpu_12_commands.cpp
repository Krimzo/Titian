#include "klibrary.h"


kl::GPU12Commands::GPU12Commands()
{}

kl::GPU12Commands::GPU12Commands( dx12::CommandAllocator const& allocator )
    : allocator( allocator )
{}

kl::GPU12Commands::GPU12Commands( dx12::CommandAllocator const& allocator, dx12::CommandList const& list )
    : allocator( allocator ), list( list )
{}

void kl::GPU12Commands::reset() const
{
    allocator->Reset() >> verify_result;
    list->Reset( allocator.get(), nullptr ) >> verify_result;
}

void kl::GPU12Commands::close() const
{
    list->Close() >> verify_result;
}

void kl::GPU12Commands::transition_resource( dx12::Resource const& resource, D3D12_RESOURCE_STATES from, D3D12_RESOURCE_STATES to ) const
{
    D3D12_RESOURCE_BARRIER barrier{
        .Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
        .Transition = {
            .pResource = resource.get(),
            .StateBefore = from,
            .StateAfter = to,
    },
    };
    list->ResourceBarrier( 1, &barrier );
}

void kl::GPU12Commands::clear_target_view( dx12::DescriptorHandle const& handle, Float4 const& color ) const
{
    list->ClearRenderTargetView( handle, &color.x, 0, nullptr );
}

void kl::GPU12Commands::copy( dx12::Resource const& destination, dx12::Resource const& source ) const
{
    list->CopyResource( destination.get(), source.get() );
}

void kl::GPU12Commands::set_root_signature( dx12::RootSignature const& root_signature ) const
{
    list->SetGraphicsRootSignature( root_signature.get() );
}

void kl::GPU12Commands::set_pipeline_state( dx12::PipelineState const& pipeline_state ) const
{
    list->SetPipelineState( pipeline_state.get() );
}

void kl::GPU12Commands::set_primitive_topology( D3D_PRIMITIVE_TOPOLOGY topology ) const
{
    list->IASetPrimitiveTopology( topology );
}

void kl::GPU12Commands::set_vertex_buffer( dx12::VertexBuffer const& vertex_buffer ) const
{
    list->IASetVertexBuffers( 0, 1, &vertex_buffer );
}

void kl::GPU12Commands::set_scissors( dx12::Scissors const& scissors ) const
{
    list->RSSetScissorRects( 1, &scissors );
}

void kl::GPU12Commands::set_viewport( dx12::Viewport const& viewport ) const
{
    list->RSSetViewports( 1, &viewport );
}

void kl::GPU12Commands::set_render_target( dx12::DescriptorHandle const* render_target, dx12::DescriptorHandle const* depth_target ) const
{
    list->OMSetRenderTargets( 1, render_target, true, depth_target );
}

void kl::GPU12Commands::draw( UINT vertex_count, UINT instance_count ) const
{
    list->DrawInstanced( vertex_count, instance_count, 0, 0 );
}

void kl::GPU12Commands::update_tlas( dx12::AccelerationStructure const& tlas, dx12::AccelerationStructure const& tlas_update_scratch, dx12::Resource const& instances ) const
{
    UINT instance_count = UINT( instances->GetDesc().Width / sizeof( D3D12_RAYTRACING_INSTANCE_DESC ) );
    D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC acceleration_descriptor{
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
    list->BuildRaytracingAccelerationStructure( &acceleration_descriptor, 0, nullptr );
}

void kl::GPU12Commands::dispatch_rays( D3D12_GPU_VIRTUAL_ADDRESS shader_address, UINT width, UINT height ) const
{
    D3D12_DISPATCH_RAYS_DESC dispatch_descriptor{
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
    list->DispatchRays( &dispatch_descriptor );
}
