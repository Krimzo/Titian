#include "klibrary.h"


kl::GPU12::GPU12(const HWND window, const bool debug)
{
	if (debug) {
		ComPtr<ID3D12Debug> debug_interface{};
		D3D12GetDebugInterface(IID_PPV_ARGS(&debug_interface)) >> verify_result;
		debug_interface->EnableDebugLayer();
	}
	D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&m_device)) >> verify_result;
	if (debug) {
		ComPtr<ID3D12DebugDevice> debug_device{};
		m_device->QueryInterface(IID_PPV_ARGS(&debug_device)) >> verify_result;
		debug_device->ReportLiveDeviceObjects(D3D12_RLDO_SUMMARY);
		debug_device.Reset();
	}

	queue = create_command_queue();
	commands.allocator = create_command_allocator();
	commands.list = create_command_list(commands.allocator);
	fence = create_fence();

	CreateDXGIFactory2(debug ? DXGI_CREATE_FACTORY_DEBUG : NULL, IID_PPV_ARGS(&m_dxgi_factor)) >> verify_result;
	m_swap_chain = create_swap_chain(window, queue.queue);

	m_rtv_descriptor_heap = create_descriptor_heap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, BACK_BUFFER_COUNT);
	m_rtv_descriptor_size = get_descriptor_size(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtv_handle{ m_rtv_descriptor_heap->GetCPUDescriptorHandleForHeapStart() };
	for (int i = 0; i < BACK_BUFFER_COUNT; i++) {
		m_swap_chain->GetBuffer(i, IID_PPV_ARGS(&m_back_buffers[i])) >> verify_result;
		m_device->CreateRenderTargetView(m_back_buffers[i].Get(), nullptr, rtv_handle);
		rtv_handle.Offset(m_rtv_descriptor_size);
	}
}

kl::dx12::Device kl::GPU12::device() const
{
	return m_device;
}

kl::dx12::SwapChain kl::GPU12::chain() const
{
	return m_swap_chain;
}

kl::dx12::DescriptorHeap kl::GPU12::rtv_heap() const
{
	return m_rtv_descriptor_heap;
}

UINT kl::GPU12::rtv_size() const
{
	return m_rtv_descriptor_size;
}

UINT kl::GPU12::back_buffer_index() const
{
	return m_swap_chain->GetCurrentBackBufferIndex();
}

kl::dx12::CommandQueue kl::GPU12::create_command_queue() const
{
	const D3D12_COMMAND_QUEUE_DESC descriptor{
		.Type = D3D12_COMMAND_LIST_TYPE_DIRECT,
		.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,
		.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE,
		.NodeMask = NULL,
	};
	dx12::CommandQueue command_queue{};
	m_device->CreateCommandQueue(&descriptor, IID_PPV_ARGS(&command_queue)) >> verify_result;
	return command_queue;
}

kl::dx12::SwapChain kl::GPU12::create_swap_chain(const HWND window, const dx12::CommandQueue& command_queue) const
{
	RECT window_client_area = {};
	GetClientRect(window, &window_client_area);

	const DXGI_SWAP_CHAIN_DESC1 descriptor{
		.Width = static_cast<UINT>(window_client_area.right),
		.Height = static_cast<UINT>(window_client_area.bottom),
		.Format = DXGI_FORMAT_R8G8B8A8_UNORM,
		.Stereo = false,
		.SampleDesc = {
			.Count = 1,
			.Quality = 0,
		},
		.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
		.BufferCount = BACK_BUFFER_COUNT,
		.Scaling = DXGI_SCALING_STRETCH,
		.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
		.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED,
		.Flags = NULL, // DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING use this for variable refresh rate monitors
	};

	dx12::SwapChain swap_chain{};
	{
		ComPtr<IDXGISwapChain1> temp_chain{};
		m_dxgi_factor->CreateSwapChainForHwnd(command_queue.Get(), window, &descriptor, nullptr, nullptr, &temp_chain) >> verify_result;
		temp_chain.As(&swap_chain) >> verify_result;
	}
	return swap_chain;
}

kl::dx12::DescriptorHeap kl::GPU12::create_descriptor_heap(const D3D12_DESCRIPTOR_HEAP_TYPE type, const UINT count) const
{
	const D3D12_DESCRIPTOR_HEAP_DESC descriptor{
		.Type = type,
		.NumDescriptors = count,
	};
	dx12::DescriptorHeap descriptor_heap{};
	m_device->CreateDescriptorHeap(&descriptor, IID_PPV_ARGS(&descriptor_heap)) >> verify_result;
	return descriptor_heap;
}

UINT kl::GPU12::get_descriptor_size(const D3D12_DESCRIPTOR_HEAP_TYPE type) const
{
	return m_device->GetDescriptorHandleIncrementSize(type);
}

kl::dx12::CommandAllocator kl::GPU12::create_command_allocator() const
{
	dx12::CommandAllocator command_allocator{};
	m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&command_allocator)) >> verify_result;
	return command_allocator;
}

kl::dx12::CommandList kl::GPU12::create_command_list(const dx12::CommandAllocator& command_allocator) const
{
	dx12::CommandList command_list{};
	m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, command_allocator.Get(), nullptr, IID_PPV_ARGS(&command_list)) >> verify_result;
	command_list->Close() >> verify_result;
	return command_list;
}

kl::dx12::Fence kl::GPU12::create_fence() const
{
	dx12::Fence fence{};
	m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)) >> verify_result;
	return fence;
}

kl::dx12::Resource kl::GPU12::get_back_buffer(const UINT index) const
{
	if (index >= BACK_BUFFER_COUNT) {
		return nullptr;
	}
	return m_back_buffers[index];
}

kl::dx12::DescriptorHandle kl::GPU12::get_render_target(const UINT index) const
{
	return { m_rtv_descriptor_heap->GetCPUDescriptorHandleForHeapStart(), static_cast<INT>(index), m_rtv_descriptor_size };
}

void kl::GPU12::swap_buffers(const bool v_sync) const
{
	m_swap_chain->Present(static_cast<UINT>(v_sync), NULL) >> verify_result;
}

kl::dx12::Resource kl::GPU12::create_commited_resource(const UINT byte_size, const D3D12_RESOURCE_STATES resource_state, const D3D12_HEAP_TYPE heap_type) const
{
	const CD3DX12_HEAP_PROPERTIES heap_props{ heap_type };
	const auto resource_desc = CD3DX12_RESOURCE_DESC::Buffer(byte_size);
	dx12::Resource resource{};
	m_device->CreateCommittedResource(&heap_props, D3D12_HEAP_FLAG_NONE, &resource_desc, resource_state, nullptr, IID_PPV_ARGS(&resource)) >> verify_result;
	return resource;
}

kl::dx12::Resource kl::GPU12::create_buffer(const void* data, const UINT byte_size)
{
	const dx12::Resource vertex_buffer = create_commited_resource(byte_size, D3D12_RESOURCE_STATE_COPY_DEST);
	{
		const dx12::Resource upload_buffer = create_commited_resource(byte_size, D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_HEAP_TYPE_UPLOAD);
		copy(data, upload_buffer, byte_size);
		commands.reset();
		commands.copy(upload_buffer, vertex_buffer);
		commands.transition_resource(vertex_buffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
		commands.close();
		queue.execute(commands.list);
		fence.signal_and_wait(queue.queue);
	}
	return vertex_buffer;
}

void kl::GPU12::copy(const void* from, const dx12::Resource& to, const UINT byte_size) const
{
	void* buffer{};
	to->Map(0, nullptr, &buffer) >> verify_result;
	memcpy(buffer, from, byte_size);
	to->Unmap(0, nullptr);
}

void kl::GPU12::copy(const dx12::Resource& from, void* to, const UINT byte_size) const
{
	void* buffer{};
	from->Map(0, nullptr, &buffer) >> verify_result;
	memcpy(to, buffer, byte_size);
	from->Unmap(0, nullptr);
}

kl::dx12::RootSignature kl::GPU12::create_root_signature(
	const std::initializer_list<D3D12_ROOT_PARAMETER>& parameters,
	const std::initializer_list<D3D12_STATIC_SAMPLER_DESC>& samplers,
	const D3D12_ROOT_SIGNATURE_FLAGS flags
) const
{
	CD3DX12_ROOT_SIGNATURE_DESC descriptor{};
	descriptor.Init((UINT) parameters.size(), parameters.begin(), (UINT) samplers.size(), samplers.begin(), flags);

	dx12::Blob signature_blob{};
	dx12::Blob error_blob{};
	const HRESULT result = D3D12SerializeRootSignature(&descriptor, D3D_ROOT_SIGNATURE_VERSION_1, &signature_blob, &error_blob);
	if (FAILED(result)) {
		if (const char* error_msg = reinterpret_cast<const char*>(error_blob->GetBufferPointer())) {
			verify(false, error_msg);
		}
		result >> verify_result;
	}

	dx12::RootSignature root_signature{};
	m_device->CreateRootSignature(NULL, signature_blob->GetBufferPointer(), signature_blob->GetBufferSize(), IID_PPV_ARGS(&root_signature)) >> verify_result;
	return root_signature;
}

kl::dx12::PipelineState kl::GPU12::create_default_rasterization_pipeline(
	const dx12::RootSignature& root_signature,
	const std::string& shader_source,
	const std::vector<std::pair<std::string, DXGI_FORMAT>>& input_layout_parts,
	const D3D12_PRIMITIVE_TOPOLOGY_TYPE primitive_topology
) const
{
	// Shaders
	const CompiledShader vertex_shader = compile_vertex_shader(shader_source);
	const CompiledShader pixel_shader = compile_pixel_shader(shader_source);

	// Input layout
	std::vector<dx12::InputLayout> input_layout{};
	input_layout.reserve(input_layout_parts.size());
	for (const auto& part : input_layout_parts) {
		input_layout.emplace_back(part.first.c_str(), 0, part.second, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0);
	}

	// Pipeline
	GPU12::DefaultRaserizationPipeline pipeline_desc{};
	pipeline_desc.root_signature = root_signature.Get();
	pipeline_desc.input_layout = { input_layout.data(), (UINT) input_layout.size() };
	pipeline_desc.primitive_topology = primitive_topology;
	pipeline_desc.vertex_shader = dx12::ShaderByteCode(vertex_shader.data.Get());
	pipeline_desc.pixel_shader = dx12::ShaderByteCode(pixel_shader.data.Get());
	pipeline_desc.render_target_formats = {
		.RTFormats = DXGI_FORMAT_R8G8B8A8_UNORM,
		.NumRenderTargets = 1,
	};
	return create_pipeline_state(&pipeline_desc);
}
