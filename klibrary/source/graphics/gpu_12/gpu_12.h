#pragma once

#include "graphics/gpu_12/gpu_12_queue.h"
#include "graphics/gpu_12/gpu_12_commands.h"
#include "graphics/gpu_12/gpu_12_fence.h"
#include "graphics/shaders/shader_compiler.h"


namespace kl {
	class GPU12 : public ShaderCompiler
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

		GPU12(const GPU12&) = delete;
		GPU12(const GPU12&&) = delete;

		void operator=(const GPU12&) = delete;
		void operator=(const GPU12&&) = delete;

		dx12::Device device() const;
		dx12::SwapChain chain() const;
		dx12::DescriptorHeap rtv_heap() const;

		UINT rtv_size() const;
		UINT back_buffer_index() const;

		dx12::CommandQueue create_command_queue() const;
		dx12::SwapChain create_swap_chain(HWND window, const dx12::CommandQueue& command_queue) const;
		dx12::DescriptorHeap create_descriptor_heap(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT count) const;

		UINT get_descriptor_size(D3D12_DESCRIPTOR_HEAP_TYPE type) const;

		dx12::CommandAllocator create_command_allocator() const;
		dx12::CommandList create_command_list(const dx12::CommandAllocator& command_allocator) const;
		dx12::Fence create_fence() const;

		dx12::Resource get_back_buffer(UINT index) const;
		dx12::DescriptorHandle get_render_target(UINT index) const;
		void swap_buffers(bool v_sync) const;

		dx12::Resource create_commited_resource(
			UINT byte_size,
			D3D12_RESOURCE_STATES resource_state,
			D3D12_HEAP_TYPE heap_type = D3D12_HEAP_TYPE_DEFAULT
		) const;
		dx12::Resource create_buffer(const void* data, UINT byte_size);

		template<typename T>
		dx12::VertexBuffer create_vertex_buffer(const dx12::Resource& resource, const UINT vertex_count) const
		{
			dx12::VertexBuffer vertex_buffer{};
			vertex_buffer.BufferLocation = resource->GetGPUVirtualAddress();
			vertex_buffer.SizeInBytes = vertex_count * sizeof(T);
			vertex_buffer.StrideInBytes = sizeof(T);
			return vertex_buffer;
		}

		void copy(const void* from, const dx12::Resource& to, UINT byte_size) const;
		void copy(const dx12::Resource& from, void* to, UINT byte_size) const;

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

		// Helpers
		template<typename T>
		std::pair<dx12::Resource, dx12::VertexBuffer> create_vertex_buffer(const T* vertices, const UINT vertex_count)
		{
			const dx12::Resource vertex_buffer = create_buffer(vertices, static_cast<UINT>(vertex_count * sizeof(T)));
			const dx12::VertexBuffer vertex_buffer_view = create_vertex_buffer<T>(vertex_buffer, vertex_count);
			return { vertex_buffer, vertex_buffer_view };
		}

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
	};
}
