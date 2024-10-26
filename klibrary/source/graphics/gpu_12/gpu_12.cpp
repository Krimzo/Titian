#include "klibrary.h"


kl::GPU12::GPU12(const HWND window, const bool debug)
{
	if (debug) {
		ComRef<ID3D12Debug> debug_interface{};
		D3D12GetDebugInterface(IID_PPV_ARGS(&debug_interface)) >> verify_result;
		debug_interface->EnableDebugLayer();
	}
	D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&m_device)) >> verify_result;

	m_queue = create_command_queue();
	m_commands.allocator = create_command_allocator();
	m_commands.list = create_command_list(m_commands.allocator);
	m_fence = create_fence();

	CreateDXGIFactory2(debug ? DXGI_CREATE_FACTORY_DEBUG : NULL, IID_PPV_ARGS(&m_dxgi_factory)) >> verify_result;
	m_swap_chain = create_swap_chain(window, m_queue.queue);

	RECT window_area{};
	GetClientRect(window, &window_area);
	resize({ (int) window_area.right, (int) window_area.bottom });
}

kl::GPU12::~GPU12()
{
	await();
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
	constexpr D3D12_COMMAND_QUEUE_DESC descriptor{
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
	RECT window_client_area{};
	GetClientRect(window, &window_client_area);

	const DXGI_SWAP_CHAIN_DESC1 descriptor{
		.Width = UINT(window_client_area.right - window_client_area.left),
		.Height = UINT(window_client_area.bottom - window_client_area.top),
		.Format = DXGI_FORMAT_R8G8B8A8_UNORM,
		.Stereo = false,
		.SampleDesc = {
			.Count = 1,
			.Quality = 0,
		},
		.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
		.BufferCount = GPU_BUFFER_COUNT,
		.Scaling = DXGI_SCALING_STRETCH,
		.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
		.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED,
		.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING,
	};

	dx12::SwapChain swap_chain;
	{
		ComRef<IDXGISwapChain1> temp_chain;
		m_dxgi_factory->CreateSwapChainForHwnd(command_queue.get(), window, &descriptor, nullptr, nullptr, &temp_chain) >> verify_result;
		temp_chain.as(swap_chain) >> verify_result;
	}
	return swap_chain;
}

kl::dx12::DescriptorHeap kl::GPU12::create_descriptor_heap(const D3D12_DESCRIPTOR_HEAP_DESC* descriptor) const
{
	dx12::DescriptorHeap descriptor_heap{};
	m_device->CreateDescriptorHeap(descriptor, IID_PPV_ARGS(&descriptor_heap)) >> verify_result;
	return descriptor_heap;
}

kl::dx12::DescriptorHeap kl::GPU12::create_descriptor_heap(const D3D12_DESCRIPTOR_HEAP_TYPE type, const UINT count, const D3D12_DESCRIPTOR_HEAP_FLAGS flags) const
{
	const D3D12_DESCRIPTOR_HEAP_DESC descriptor{
		.Type = type,
		.NumDescriptors = count,
		.Flags = flags,
	};
	return create_descriptor_heap(&descriptor);
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
	m_device->CreateCommandList1(0, D3D12_COMMAND_LIST_TYPE_DIRECT, D3D12_COMMAND_LIST_FLAG_NONE, IID_PPV_ARGS(&command_list)) >> verify_result;
	return command_list;
}

kl::dx12::Fence kl::GPU12::create_fence() const
{
	dx12::Fence fence{};
	m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)) >> verify_result;
	return fence;
}

void kl::GPU12::execute(const std::function<void(GPU12Commands&)>& func)
{
	m_commands.reset();
	func(m_commands);
	m_commands.close();
	m_queue.execute(m_commands.list);
	await();
}

void kl::GPU12::await()
{
	m_fence.signal_and_wait(m_queue.queue);
}

void kl::GPU12::resize(const Int2 size)
{
	await();

	for (auto& back_buffer : m_back_buffers) {
		back_buffer = {};
	}
	m_rtv_descriptor_heap = {};
	m_rtv_descriptor_size = 0;

	m_swap_chain->ResizeBuffers(0, (UINT) size.x, (UINT) size.y, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING);
	m_rtv_descriptor_heap = create_descriptor_heap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, GPU_BUFFER_COUNT);
	m_rtv_descriptor_size = get_descriptor_size(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	dx12::DescriptorHandle rtv_handle = m_rtv_descriptor_heap->GetCPUDescriptorHandleForHeapStart();
	for (int i = 0; i < GPU_BUFFER_COUNT; i++) {
		m_swap_chain->GetBuffer(i, IID_PPV_ARGS(&m_back_buffers[i])) >> verify_result;
		m_device->CreateRenderTargetView(m_back_buffers[i].get(), nullptr, rtv_handle);
		rtv_handle.ptr += m_rtv_descriptor_size;
	}
}

kl::dx12::Resource kl::GPU12::get_back_buffer(const UINT index) const
{
	return m_back_buffers[index];
}

kl::dx12::DescriptorHandle kl::GPU12::get_render_target(const UINT index) const
{
	return { .ptr = m_rtv_descriptor_heap->GetCPUDescriptorHandleForHeapStart().ptr + (SIZE_T) m_rtv_descriptor_size * index };
}

void kl::GPU12::swap_buffers(const bool v_sync) const
{
	const UINT interval = v_sync ? 1 : 0;
	const UINT flags = v_sync ? NULL : DXGI_PRESENT_ALLOW_TEARING;
	m_swap_chain->Present(interval, flags) >> verify_result;
}

kl::dx12::Resource kl::GPU12::create_commited_resource(const D3D12_RESOURCE_DESC* descriptor, const D3D12_RESOURCE_STATES resource_state, const D3D12_HEAP_TYPE heap_type) const
{
	const D3D12_HEAP_PROPERTIES heap_props{ .Type = heap_type };
	dx12::Resource resource{};
	m_device->CreateCommittedResource(&heap_props, D3D12_HEAP_FLAG_NONE, descriptor, resource_state, nullptr, IID_PPV_ARGS(&resource)) >> verify_result;
	return resource;
}

kl::dx12::Resource kl::GPU12::create_commited_resource(const UINT64 byte_size, const D3D12_RESOURCE_STATES resource_state, const D3D12_HEAP_TYPE heap_type, const D3D12_RESOURCE_FLAGS flags) const
{
	const D3D12_RESOURCE_DESC resource_desc{
		.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER,
		.Alignment = 0,
		.Width = byte_size,
		.Height = 1,
		.DepthOrArraySize = 1,
		.MipLevels = 1,
		.Format = DXGI_FORMAT_UNKNOWN,
		.SampleDesc = { 1, 0 },
		.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
		.Flags = flags,
	};
	return create_commited_resource(&resource_desc, resource_state, heap_type);
}

kl::dx12::Resource kl::GPU12::create_upload_buffer(const void* data, const UINT byte_size) const
{
	dx12::Resource buffer = create_commited_resource(byte_size, D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_HEAP_TYPE_UPLOAD);
	copy(buffer, data, byte_size);
	return buffer;
}

kl::dx12::Resource kl::GPU12::create_buffer(const void* data, const UINT byte_size, const D3D12_RESOURCE_STATES final_state)
{
	dx12::Resource buffer = create_commited_resource(byte_size, D3D12_RESOURCE_STATE_COMMON);
	const dx12::Resource upload_buffer = create_upload_buffer(data, byte_size);
	execute([&](auto& commands)
	{
		commands.transition_resource(buffer, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
		commands.copy(buffer, upload_buffer);
		commands.transition_resource(buffer, D3D12_RESOURCE_STATE_COPY_DEST, final_state);
	});
	return buffer;
}

void kl::GPU12::copy(const dx12::Resource& destination, const void* source, const UINT byte_size) const
{
	void* buffer = nullptr;
	destination->Map(0, nullptr, &buffer) >> verify_result;
	kl::copy<byte>(buffer, source, byte_size);
	destination->Unmap(0, nullptr);
}

void kl::GPU12::copy(void* destination, const dx12::Resource& source, const UINT byte_size) const
{
	void* buffer = nullptr;
	source->Map(0, nullptr, &buffer) >> verify_result;
	kl::copy<byte>(destination, buffer, byte_size);
	source->Unmap(0, nullptr);
}

kl::dx12::RootSignature kl::GPU12::create_root_signature(const std::initializer_list<D3D12_ROOT_PARAMETER>& parameters, const std::initializer_list<D3D12_STATIC_SAMPLER_DESC>& samplers, const D3D12_ROOT_SIGNATURE_FLAGS flags) const
{
	const D3D12_ROOT_SIGNATURE_DESC descriptor{
		.NumParameters = (UINT) parameters.size(),
		.pParameters = parameters.begin(),
		.NumStaticSamplers = (UINT) samplers.size(),
		.pStaticSamplers = samplers.begin(),
		.Flags = flags,
	};
	dx12::Blob signature_blob{};
	dx12::Blob error_blob{};
	const HRESULT result = D3D12SerializeRootSignature(&descriptor, D3D_ROOT_SIGNATURE_VERSION_1, &signature_blob, &error_blob);
	if (FAILED(result)) {
		if (const char* error_msg = (const char*) error_blob->GetBufferPointer()) {
			verify(false, error_msg);
		}
		result >> verify_result;
	}
	dx12::RootSignature root_signature{};
	m_device->CreateRootSignature(NULL, signature_blob->GetBufferPointer(), signature_blob->GetBufferSize(), IID_PPV_ARGS(&root_signature)) >> verify_result;
	return root_signature;
}

kl::dx12::AccelerationStructure kl::GPU12::create_acceleration_structure(const dx12::AccelerationInputs& inputs, UINT64* update_scratch_size)
{
	D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO prebuild_info{};
	m_device->GetRaytracingAccelerationStructurePrebuildInfo(&inputs, &prebuild_info);
	if (update_scratch_size) {
		*update_scratch_size = prebuild_info.UpdateScratchDataSizeInBytes;
	}
	const dx12::Resource scratch = create_commited_resource(prebuild_info.ScratchDataSizeInBytes, D3D12_RESOURCE_STATE_COMMON, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
	const dx12::Resource acceleration_structure = create_commited_resource(prebuild_info.ResultDataMaxSizeInBytes, D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
	const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC acceleration_descriptor{
		.DestAccelerationStructureData = acceleration_structure->GetGPUVirtualAddress(),
		.Inputs = inputs,
		.ScratchAccelerationStructureData = scratch->GetGPUVirtualAddress(),
	};
	execute([&](auto& commands)
	{
		commands.transition_resource(scratch, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
		commands.list->BuildRaytracingAccelerationStructure(&acceleration_descriptor, 0, nullptr);
	});
	return acceleration_structure;
}

kl::dx12::AccelerationStructure kl::GPU12::create_blas(const D3D12_RAYTRACING_GEOMETRY_DESC* geometry_descriptor, UINT64* update_scratch_size)
{
	const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS inputs{
		.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL,
		.Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE,
		.NumDescs = 1,
		.DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY,
		.pGeometryDescs = geometry_descriptor,
	};
	return create_acceleration_structure(inputs, update_scratch_size);
}

kl::dx12::AccelerationStructure kl::GPU12::create_triangle_blas(const dx12::Resource& vertex_buffer, const UINT vertex_stride)
{
	const UINT vertex_count = UINT(vertex_buffer->GetDesc().Width / vertex_stride);
	const D3D12_RAYTRACING_GEOMETRY_DESC geometry_descriptor{
		.Type = D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES,
		.Flags = D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE,
		.Triangles{
			.Transform3x4 = NULL,
			.IndexFormat = DXGI_FORMAT_UNKNOWN,
			.VertexFormat = DXGI_FORMAT_R32G32B32_FLOAT,
			.IndexCount = 0,
			.VertexCount = vertex_count,
			.IndexBuffer = NULL,
			.VertexBuffer{
				.StartAddress = vertex_buffer->GetGPUVirtualAddress(),
				.StrideInBytes = (UINT64) vertex_stride,
			},
		},
	};
	return create_blas(&geometry_descriptor);
}

kl::dx12::AccelerationStructure kl::GPU12::create_tlas(const dx12::Resource& instances, UINT64* update_scratch_size)
{
	const UINT instance_count = UINT(instances->GetDesc().Width / sizeof(D3D12_RAYTRACING_INSTANCE_DESC));
	const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS inputs{
		.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL,
		.Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_ALLOW_UPDATE,
		.NumDescs = instance_count,
		.DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY,
		.InstanceDescs = instances->GetGPUVirtualAddress(),
	};
	return create_acceleration_structure(inputs, update_scratch_size);
}

kl::dx12::PipelineState kl::GPU12::create_default_rasterization_pipeline(const dx12::RootSignature& root_signature, const std::string_view& shader_source, const std::vector<std::pair<std::string, DXGI_FORMAT>>& input_layout_parts, const D3D12_PRIMITIVE_TOPOLOGY_TYPE primitive_topology) const
{
	const CompiledShader vertex_shader = compile_vertex_shader(shader_source);
	const CompiledShader pixel_shader = compile_pixel_shader(shader_source);

	std::vector<dx12::InputLayout> input_layout{};
	input_layout.reserve(input_layout_parts.size());
	for (const auto& [name, format] : input_layout_parts) {
		input_layout.emplace_back(name.data(), 0, format, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0);
	}

	struct RasterPipeline
	{
		dx12::SubobjectPair<D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_ROOT_SIGNATURE, ID3D12RootSignature*> root_signature;
		dx12::SubobjectPair<D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_INPUT_LAYOUT, D3D12_INPUT_LAYOUT_DESC> input_layout;
		dx12::SubobjectPair<D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_PRIMITIVE_TOPOLOGY, D3D12_PRIMITIVE_TOPOLOGY_TYPE> primitive_topology;
		dx12::SubobjectPair<D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_VS, D3D12_SHADER_BYTECODE> vertex_shader;
		dx12::SubobjectPair<D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_PS, D3D12_SHADER_BYTECODE> pixel_shader;
		dx12::SubobjectPair<D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_RENDER_TARGET_FORMATS, D3D12_RT_FORMAT_ARRAY> render_target_formats;
	};

	RasterPipeline pipeline{
		.root_signature = root_signature.get(),
		.input_layout = D3D12_INPUT_LAYOUT_DESC{
			.pInputElementDescs = input_layout.data(),
			.NumElements = (UINT) input_layout.size(),
		},
		.primitive_topology = primitive_topology,
		.vertex_shader = D3D12_SHADER_BYTECODE{
			.pShaderBytecode = vertex_shader.data_ptr(),
			.BytecodeLength = vertex_shader.data_size(),
		},
		.pixel_shader = D3D12_SHADER_BYTECODE{
			.pShaderBytecode = pixel_shader.data_ptr(),
			.BytecodeLength = pixel_shader.data_size(),
		},
		.render_target_formats = D3D12_RT_FORMAT_ARRAY{
			.RTFormats = DXGI_FORMAT_R8G8B8A8_UNORM,
			.NumRenderTargets = 1,
		},
	};
	return create_pipeline_state<RasterPipeline>(&pipeline);
}

kl::dx12::StateObject kl::GPU12::create_default_raytracing_pipeline(const std::string_view& compiled_shaders, const dx12::RootSignature& root_signature, const UINT max_recursion_depth, const UINT max_attribute_size, const UINT max_payload_size) const
{
	const D3D12_DXIL_LIBRARY_DESC dxil_lib{
		.DXILLibrary{
			.pShaderBytecode = (const void*) compiled_shaders.data(),
			.BytecodeLength = (SIZE_T) compiled_shaders.size(),
		},
	};
	constexpr D3D12_HIT_GROUP_DESC hit_group{
		.HitGroupExport = L"hit_group",
		.Type = D3D12_HIT_GROUP_TYPE_TRIANGLES,
		.ClosestHitShaderImport = L"closest_hit_shader",
	};
	const D3D12_RAYTRACING_SHADER_CONFIG shader_config{
		.MaxPayloadSizeInBytes = max_payload_size,
		.MaxAttributeSizeInBytes = max_attribute_size,
	};
	const D3D12_GLOBAL_ROOT_SIGNATURE global_signature{
		root_signature.get(),
	};
	const D3D12_RAYTRACING_PIPELINE_CONFIG pipeline_config{
		.MaxTraceRecursionDepth = max_recursion_depth,
	};

	const D3D12_STATE_SUBOBJECT subobjects[5] = {
		{ .Type = D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY, .pDesc = &dxil_lib },
		{ .Type = D3D12_STATE_SUBOBJECT_TYPE_HIT_GROUP, .pDesc = &hit_group },
		{ .Type = D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_SHADER_CONFIG, .pDesc = &shader_config },
		{ .Type = D3D12_STATE_SUBOBJECT_TYPE_GLOBAL_ROOT_SIGNATURE, .pDesc = &global_signature },
		{ .Type = D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_PIPELINE_CONFIG, .pDesc = &pipeline_config },
	};
	const D3D12_STATE_OBJECT_DESC object_descriptor{
		.Type = D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE,
		.NumSubobjects = (UINT) std::size(subobjects),
		.pSubobjects = subobjects,
	};

	dx12::StateObject pipeline_state{};
	m_device->CreateStateObject(&object_descriptor, IID_PPV_ARGS(&pipeline_state)) >> verify_result;
	return pipeline_state;
}
