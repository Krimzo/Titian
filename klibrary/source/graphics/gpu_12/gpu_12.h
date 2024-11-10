#pragma once

#include "graphics/gpu_12/gpu_12_queue.h"
#include "graphics/gpu_12/gpu_12_commands.h"
#include "graphics/gpu_12/gpu_12_fence.h"
#include "graphics/shaders/shader_compiler.h"


namespace kl::dx12
{
template<D3D12_PIPELINE_STATE_SUBOBJECT_TYPE Type, typename Value>
struct alignas(void*) SubobjectPair
{
    D3D12_PIPELINE_STATE_SUBOBJECT_TYPE type = Type;
    Value value;
    SubobjectPair( Value value ) : value( value ) {}
};
}

namespace kl
{
struct GPU12 : NoCopy, ShaderCompiler
{
    GPU12( HWND window, bool debug = IS_DEBUG );
    virtual ~GPU12();

    dx12::Device device() const;
    dx12::SwapChain chain() const;
    dx12::DescriptorHeap rtv_heap() const;

    UINT rtv_size() const;
    UINT back_buffer_index() const;

    dx12::CommandQueue create_command_queue() const;
    dx12::SwapChain create_swap_chain( HWND window, dx12::CommandQueue const& command_queue ) const;

    dx12::DescriptorHeap create_descriptor_heap( D3D12_DESCRIPTOR_HEAP_DESC const* descriptor ) const;
    dx12::DescriptorHeap create_descriptor_heap( D3D12_DESCRIPTOR_HEAP_TYPE type, UINT count, D3D12_DESCRIPTOR_HEAP_FLAGS flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE ) const;
    UINT get_descriptor_size( D3D12_DESCRIPTOR_HEAP_TYPE type ) const;

    dx12::CommandAllocator create_command_allocator() const;
    dx12::CommandList create_command_list( dx12::CommandAllocator const& command_allocator ) const;
    dx12::Fence create_fence() const;

    void execute( std::function<void( GPU12Commands& )> const& func );
    void await();

    void resize( Int2 size );
    dx12::Resource get_back_buffer( UINT index ) const;
    dx12::DescriptorHandle get_render_target( UINT index ) const;
    void swap_buffers( bool v_sync ) const;

    dx12::Resource create_commited_resource(
        D3D12_RESOURCE_DESC const* descriptor,
        D3D12_RESOURCE_STATES resource_state,
        D3D12_HEAP_TYPE heap_type = D3D12_HEAP_TYPE_DEFAULT
    ) const;

    dx12::Resource create_commited_resource(
        UINT64 byte_size,
        D3D12_RESOURCE_STATES resource_state,
        D3D12_HEAP_TYPE heap_type = D3D12_HEAP_TYPE_DEFAULT,
        D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE
    ) const;

    dx12::Resource create_upload_buffer( void const* data, UINT byte_size ) const;
    dx12::Resource create_buffer( void const* data, UINT byte_size, D3D12_RESOURCE_STATES final_state = D3D12_RESOURCE_STATE_COMMON );

    template<typename T>
    dx12::VertexBuffer create_vertex_buffer( dx12::Resource const& resource ) const
    {
        dx12::VertexBuffer vertex_buffer{};
        vertex_buffer.BufferLocation = resource->GetGPUVirtualAddress();
        vertex_buffer.SizeInBytes = (UINT) resource->GetDesc().Width;
        vertex_buffer.StrideInBytes = (UINT) sizeof( T );
        return vertex_buffer;
    }

    template<typename T>
    std::pair<dx12::Resource, dx12::VertexBuffer> create_vertex_buffer( T const* vertices, UINT vertex_count )
    {
        dx12::Resource vertex_buffer = create_buffer( vertices, UINT( vertex_count * sizeof( T ) ) );
        dx12::VertexBuffer vertex_buffer_view = create_vertex_buffer<T>( vertex_buffer );
        return { vertex_buffer, vertex_buffer_view };
    }

    void copy( dx12::Resource const& destination, void const* source, UINT byte_size ) const;
    void copy( void* destination, dx12::Resource const& source, UINT byte_size ) const;

    dx12::RootSignature create_root_signature(
        std::initializer_list<D3D12_ROOT_PARAMETER> const& parameters = {},
        std::initializer_list<D3D12_STATIC_SAMPLER_DESC> const& samplers = {},
        D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
    ) const;

    template<typename T>
    dx12::PipelineState create_pipeline_state( T const* pipeline_state_desc ) const
    {
        D3D12_PIPELINE_STATE_STREAM_DESC descriptor{
            .SizeInBytes = (SIZE_T) sizeof( T ),
            .pPipelineStateSubobjectStream = (void*) pipeline_state_desc,
        };
        dx12::PipelineState pipeline_state{};
        m_device->CreatePipelineState( &descriptor, IID_PPV_ARGS( &pipeline_state ) ) >> verify_result;
        return pipeline_state;
    }

    dx12::AccelerationStructure create_acceleration_structure( dx12::AccelerationInputs const& inputs, UINT64* update_scratch_size = nullptr );
    dx12::AccelerationStructure create_blas( D3D12_RAYTRACING_GEOMETRY_DESC const* geometry_descriptor, UINT64* update_scratch_size = nullptr );
    dx12::AccelerationStructure create_triangle_blas( dx12::Resource const& vertex_buffer, UINT vertex_stride = sizeof( kl::Vertex ) );
    dx12::AccelerationStructure create_tlas( dx12::Resource const& instances, UINT64* update_scratch_size = nullptr );

    dx12::PipelineState create_default_rasterization_pipeline(
        kl::dx12::RootSignature const& root_signature,
        std::string_view const& shader_source,
        std::vector<std::pair<std::string, DXGI_FORMAT>> const& input_layout_parts = {
            { "KL_Position", DXGI_FORMAT_R32G32B32_FLOAT },
        { "KL_Normal", DXGI_FORMAT_R32G32B32_FLOAT },
        { "KL_UV", DXGI_FORMAT_R32G32_FLOAT },
        },
        D3D12_PRIMITIVE_TOPOLOGY_TYPE primitive_topology = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
        ) const;

    dx12::StateObject create_default_raytracing_pipeline(
        std::string_view const& compiled_shaders,
        dx12::RootSignature const& root_signature,
        UINT max_recursion_depth = 5,
        UINT max_attribute_size = sizeof( kl::Vertex ),
        UINT max_payload_size = 16
    ) const;

private:
    dx12::Device m_device{};
    dx12::DXGIFactory m_dxgi_factory{};
    dx12::SwapChain m_swap_chain{};

    GPU12Queue m_queue{};
    GPU12Commands m_commands{};
    GPU12Fence m_fence{};

    dx12::Resource m_back_buffers[GPU_BUFFER_COUNT] = {};
    dx12::DescriptorHeap m_rtv_descriptor_heap{};
    UINT m_rtv_descriptor_size{};
};
}
