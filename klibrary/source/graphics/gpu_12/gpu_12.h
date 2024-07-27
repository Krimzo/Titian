#pragma once

#include "graphics/gpu_12/gpu_12_queue.h"
#include "graphics/gpu_12/gpu_12_commands.h"
#include "graphics/gpu_12/gpu_12_fence.h"
#include "graphics/shaders/shader_compiler.h"


namespace kl {
	class GPU12 : NoCopy, public ShaderCompiler
	{
	public:
		static constexpr UINT BACK_BUFFER_COUNT = 2;

		struct DefaultRaserizationPipeline
		{
			CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE root_signature{};
			CD3DX12_PIPELINE_STATE_STREAM_INPUT_LAYOUT input_layout{};
			CD3DX12_PIPELINE_STATE_STREAM_PRIMITIVE_TOPOLOGY primitive_topology{};
			CD3DX12_PIPELINE_STATE_STREAM_VS vertex_shader{};
			CD3DX12_PIPELINE_STATE_STREAM_PS pixel_shader{};
			CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS render_target_formats{};
		};

	private:
		dx12::Device m_device{};
		dx12::DXGIFactory m_dxgi_factor{};
		dx12::SwapChain m_swap_chain{};

		dx12::Resource m_back_buffers[BACK_BUFFER_COUNT] = {};
		dx12::DescriptorHeap m_rtv_descriptor_heap{};
		UINT m_rtv_descriptor_size{};

	public:
		GPU12Queue queue{};
		GPU12Commands commands{};
		GPU12Fence fence{};

		GPU12(HWND window, bool debug = false);

		dx12::Device device() const;
		dx12::SwapChain chain() const;
		dx12::DescriptorHeap rtv_heap() const;

		UINT rtv_size() const;
		UINT back_buffer_index() const;

		dx12::CommandQueue create_command_queue() const;
		dx12::SwapChain create_swap_chain(HWND window, const dx12::CommandQueue& command_queue) const;

		dx12::DescriptorHeap create_descriptor_heap(const D3D12_DESCRIPTOR_HEAP_DESC* descriptor) const;
		dx12::DescriptorHeap create_descriptor_heap(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT count, D3D12_DESCRIPTOR_HEAP_FLAGS flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE) const;
		UINT get_descriptor_size(D3D12_DESCRIPTOR_HEAP_TYPE type) const;

		dx12::CommandAllocator create_command_allocator() const;
		dx12::CommandList create_command_list(const dx12::CommandAllocator& command_allocator) const;
		dx12::Fence create_fence() const;

		void reset();
		void execute();
		void wait();
		void execute_and_wait();

		void resize(const kl::Int2& size);
		dx12::Resource get_back_buffer(UINT index) const;
		dx12::DescriptorHandle get_render_target(UINT index) const;
		void swap_buffers(bool v_sync) const;

		dx12::Resource create_commited_resource(
			const D3D12_RESOURCE_DESC* descriptor,
			D3D12_RESOURCE_STATES resource_state,
			D3D12_HEAP_TYPE heap_type = D3D12_HEAP_TYPE_DEFAULT
		) const;

		dx12::Resource create_commited_resource(
			UINT64 byte_size,
			D3D12_RESOURCE_STATES resource_state,
			D3D12_HEAP_TYPE heap_type = D3D12_HEAP_TYPE_DEFAULT,
			D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE
		) const;

		dx12::Resource create_upload_buffer(const void* data, UINT byte_size) const;
		dx12::Resource create_buffer(const void* data, UINT byte_size, D3D12_RESOURCE_STATES final_state = D3D12_RESOURCE_STATE_COMMON);

		template<typename T>
		dx12::VertexBuffer create_vertex_buffer(const dx12::Resource& resource, const UINT vertex_count) const
		{
			dx12::VertexBuffer vertex_buffer{};
			vertex_buffer.BufferLocation = resource->GetGPUVirtualAddress();
			vertex_buffer.SizeInBytes = vertex_count * sizeof(T);
			vertex_buffer.StrideInBytes = sizeof(T);
			return vertex_buffer;
		}

		template<typename T>
		std::pair<dx12::Resource, dx12::VertexBuffer> create_vertex_buffer(const T* vertices, const UINT vertex_count)
		{
			const dx12::Resource vertex_buffer = create_buffer(vertices, static_cast<UINT>(vertex_count * sizeof(T)));
			const dx12::VertexBuffer vertex_buffer_view = create_vertex_buffer<T>(vertex_buffer, vertex_count);
			return { vertex_buffer, vertex_buffer_view };
		}

		void copy(dx12::Resource& destination, const void* source, UINT byte_size) const;
		void copy(void* destination, const dx12::Resource& source, UINT byte_size) const;

		dx12::RootSignature create_root_signature(
			const std::initializer_list<D3D12_ROOT_PARAMETER>& parameters = {},
			const std::initializer_list<D3D12_STATIC_SAMPLER_DESC>& samplers = {},
			D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
		) const;

		template<typename T>
		dx12::PipelineState create_pipeline_state(T* pipeline_state_desc) const
		{
			const D3D12_PIPELINE_STATE_STREAM_DESC descriptor{
				.SizeInBytes{ sizeof(T) },
				.pPipelineStateSubobjectStream{ pipeline_state_desc },
			};

			dx12::PipelineState pipeline_state{};
			m_device->CreatePipelineState(&descriptor, IID_PPV_ARGS(&pipeline_state)) >> verify_result;
			return pipeline_state;
		}

		// Raytracing
		dx12::AccelerationStructure create_acceleration_structure(const dx12::AccelerationInputs& inputs, UINT64* update_scratch_size = nullptr);
		dx12::AccelerationStructure create_tlas(const dx12::Resource& instances, UINT instance_count, UINT64* update_scratch_size = nullptr);
		dx12::AccelerationStructure create_blas(const D3D12_RAYTRACING_GEOMETRY_DESC* geometry_descriptor, UINT64* update_scratch_size = nullptr);
		dx12::AccelerationStructure create_triangle_blas(const dx12::Resource& vertex_buffer, const dx12::Resource& index_buffer = {}, UINT vertex_stride = sizeof(kl::Vertex<float>), UINT index_stride = sizeof(uint16_t));

		void dispatch_rays(D3D12_GPU_VIRTUAL_ADDRESS shader_address, UINT width, UINT height);

		// Helpers
		dx12::PipelineState create_default_rasterization_pipeline(
			const kl::dx12::RootSignature& root_signature,
			const std::string& shader_source,
			const std::vector<std::pair<std::string, DXGI_FORMAT>>& input_layout_parts = {
				{ "KL_Position", DXGI_FORMAT_R32G32B32_FLOAT },
				{ "KL_Texture", DXGI_FORMAT_R32G32_FLOAT },
				{ "KL_Normal", DXGI_FORMAT_R32G32B32_FLOAT },
			},
			D3D12_PRIMITIVE_TOPOLOGY_TYPE primitive_topology = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		) const;

		dx12::StateObject create_default_raytracing_pipeline(
			const std::vector<byte>& compiled_shaders,
			const dx12::RootSignature& root_signature,
			UINT max_recursion_depth = 5,
			UINT max_attribute_size = sizeof(kl::Vertex<float>),
			UINT max_payload_size = 16
		) const;
	};
}
