#pragma once

#include "apis/kl_windows.h"
#include "memory/safety/com_ref.h"

#include <wrl.h>
#include <gdiplus.h>
#include <dxgi1_6.h>
#include <d2d1.h>
#include <d3d9.h>
#include <d3d11_4.h>
#include <d3d12.h>
#include <d3dcompiler.h>
#include <dwrite.h>
#include <dxva2api.h>

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dwrite.lib")


namespace kl {
    using TextFormat = ComRef<IDWriteTextFormat>;
}

namespace kl::dx {
    using Device = ComRef<ID3D11Device4>;
    using Context = ComRef<ID3D11DeviceContext4>;
    using Chain = ComRef<IDXGISwapChain4>;

    using Buffer = ComRef<ID3D11Buffer>;
    using Texture = ComRef<ID3D11Texture2D>;
    using Resource = ComRef<ID3D11Resource>;

    using RasterState = ComRef<ID3D11RasterizerState>;
    using DepthState = ComRef<ID3D11DepthStencilState>;
    using SamplerState = ComRef<ID3D11SamplerState>;
    using BlendState = ComRef<ID3D11BlendState>;

    using TargetView = ComRef<ID3D11RenderTargetView>;
    using DepthView = ComRef<ID3D11DepthStencilView>;
    using ShaderView = ComRef<ID3D11ShaderResourceView>;
    using AccessView = ComRef<ID3D11UnorderedAccessView>;

    using DataBlob = ComRef<ID3DBlob>;
    using InputLayout = ComRef<ID3D11InputLayout>;

    using VertexShader = ComRef<ID3D11VertexShader>;
    using PixelShader = ComRef<ID3D11PixelShader>;
    using GeometryShader = ComRef<ID3D11GeometryShader>;
    using HullShader = ComRef<ID3D11HullShader>;
    using DomainShader = ComRef<ID3D11DomainShader>;
    using ComputeShader = ComRef<ID3D11ComputeShader>;

    using BufferDescriptor = D3D11_BUFFER_DESC;
    using TextureDescriptor = D3D11_TEXTURE2D_DESC;
    using SubresourceDescriptor = D3D11_SUBRESOURCE_DATA;
    using MappedSubresourceDescriptor = D3D11_MAPPED_SUBRESOURCE;

    using LayoutDescriptor = D3D11_INPUT_ELEMENT_DESC;

    using RasterStateDescriptor = D3D11_RASTERIZER_DESC;
    using DepthStateDescriptor = D3D11_DEPTH_STENCIL_DESC;
    using SamplerStateDescriptor = D3D11_SAMPLER_DESC;
    using BlendStateDescriptor = D3D11_BLEND_DESC;

    using TargetViewDescriptor = D3D11_RENDER_TARGET_VIEW_DESC;
    using DepthViewDescriptor = D3D11_DEPTH_STENCIL_VIEW_DESC;
    using ShaderViewDescriptor = D3D11_SHADER_RESOURCE_VIEW_DESC;
    using AccessViewDescriptor = D3D11_UNORDERED_ACCESS_VIEW_DESC;
}

namespace kl::dx12 {
    using Device = ComRef<ID3D12Device5>;

    using CommandQueue = ComRef<ID3D12CommandQueue>;
    using CommandAllocator = ComRef<ID3D12CommandAllocator>;
    using CommandList = ComRef<ID3D12GraphicsCommandList4>;
    using Fence = ComRef<ID3D12Fence>;

    using DXGIFactory = ComRef<IDXGIFactory2>;
    using SwapChain = ComRef<IDXGISwapChain3>;

    using DescriptorHeap = ComRef<ID3D12DescriptorHeap>;
    using DescriptorHandle = D3D12_CPU_DESCRIPTOR_HANDLE;
    using Resource = ComRef<ID3D12Resource>;
    using VertexBuffer = D3D12_VERTEX_BUFFER_VIEW;

    using Blob = ComRef<ID3DBlob>;
    using ShaderByteCode = D3D12_SHADER_BYTECODE;
    using InputLayout = D3D12_INPUT_ELEMENT_DESC;
    using RootSignature = ComRef<ID3D12RootSignature>;
    using PipelineState = ComRef<ID3D12PipelineState>;
    using StateObject = ComRef<ID3D12StateObject>;

    using Scissors = D3D12_RECT;
    using Viewport = D3D12_VIEWPORT;

    using AccelerationInputs = D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS;
    using AccelerationStructure = ComRef<ID3D12Resource>;
    using ObjectProperties = ComRef<ID3D12StateObjectProperties>;
}
