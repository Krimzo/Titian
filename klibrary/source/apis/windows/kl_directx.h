#pragma once

// Other
#include "apis/windows/kl_windows.h"
#include "apis/external/kl_dx12.h"

// DirectX
#include <wrl.h>
#include <gdiplus.h>
#include <dxgi1_6.h>
#include <d2d1.h>
#include <d3d11.h>
#include <d3d12.h>
#include <d3dcompiler.h>
#include <dwrite.h>

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dwrite.lib")

namespace kl {
    using Microsoft::WRL::ComPtr;
}

namespace kl::dx {
    /* Types */
    // Device
    using Device = ComPtr<ID3D11Device>;
    using Context = ComPtr<ID3D11DeviceContext>;
    using Chain = ComPtr<IDXGISwapChain4>;

    // Data
    using Buffer = ComPtr<ID3D11Buffer>;
    using Texture = ComPtr<ID3D11Texture2D>;
    using Resource = ComPtr<ID3D11Resource>;

    // State
    using RasterState = ComPtr<ID3D11RasterizerState>;
    using DepthState = ComPtr<ID3D11DepthStencilState>;
    using SamplerState = ComPtr<ID3D11SamplerState>;
    using BlendState = ComPtr<ID3D11BlendState>;

    // View
    using TargetView = ComPtr<ID3D11RenderTargetView>;
    using DepthView = ComPtr<ID3D11DepthStencilView>;
    using ShaderView = ComPtr<ID3D11ShaderResourceView>;
    using AccessView = ComPtr<ID3D11UnorderedAccessView>;

    // Shader data
    using DataBlob = ComPtr<ID3DBlob>;
    using InputLayout = ComPtr<ID3D11InputLayout>;

    // Shaders
    using VertexShader = ComPtr<ID3D11VertexShader>;
    using PixelShader = ComPtr<ID3D11PixelShader>;
    using GeometryShader = ComPtr<ID3D11GeometryShader>;
    using HullShader = ComPtr<ID3D11HullShader>;
    using DomainShader = ComPtr<ID3D11DomainShader>;
    using ComputeShader = ComPtr<ID3D11ComputeShader>;

    /* Descriptors */
    // Data
    using BufferDescriptor = D3D11_BUFFER_DESC;
    using TextureDescriptor = D3D11_TEXTURE2D_DESC;
    using SubresourceDescriptor = D3D11_SUBRESOURCE_DATA;
    using MappedSubresourceDescriptor = D3D11_MAPPED_SUBRESOURCE;

    // Input
    using LayoutDescriptor = D3D11_INPUT_ELEMENT_DESC;

    // State
    using RasterStateDescriptor = D3D11_RASTERIZER_DESC;
    using DepthStateDescriptor = D3D11_DEPTH_STENCIL_DESC;
    using SamplerStateDescriptor = D3D11_SAMPLER_DESC;
    using BlendStateDescriptor = D3D11_BLEND_DESC;

    // View
    using TargetViewDescriptor = D3D11_RENDER_TARGET_VIEW_DESC;
    using DepthViewDescriptor = D3D11_DEPTH_STENCIL_VIEW_DESC;
    using ShaderViewDescriptor = D3D11_SHADER_RESOURCE_VIEW_DESC;
    using AccessViewDescriptor = D3D11_UNORDERED_ACCESS_VIEW_DESC;
}

namespace kl::dx12 {
    // Main device
    using Device = ComPtr<ID3D12Device8>;

    // Context commands
    using CommandQueue = ComPtr<ID3D12CommandQueue>;
    using CommandAllocator = ComPtr<ID3D12CommandAllocator>;
    using CommandList = ComPtr<ID3D12GraphicsCommandList4>;
    using Fence = ComPtr<ID3D12Fence>;

    // Swapchain
    using DXGIFactory = ComPtr<IDXGIFactory4>;
    using SwapChain = ComPtr<IDXGISwapChain4>;

    // Resources
    using DescriptorHeap = ComPtr<ID3D12DescriptorHeap>;
    using DescriptorHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE;
    using Resource = ComPtr<ID3D12Resource>;
    using VertexBuffer = D3D12_VERTEX_BUFFER_VIEW;

    // Pipeline stuff
    using Blob = ComPtr<ID3DBlob>;
    using ShaderByteCode = CD3DX12_SHADER_BYTECODE;
    using InputLayout = D3D12_INPUT_ELEMENT_DESC;
    using RootSignature = ComPtr<ID3D12RootSignature>;
    using PipelineState = ComPtr<ID3D12PipelineState>;
    using StateObject = ComPtr<ID3D12StateObject>;

    // Clippers
    using Scissors = CD3DX12_RECT;
    using Viewport = CD3DX12_VIEWPORT;

    // Raytracing
    using AccelerationInputs = D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS;
    using AccelerationStructure = ComPtr<ID3D12Resource>;
    using ObjectProperties = ComPtr<ID3D12StateObjectProperties>;
}
