#pragma once

// Windows
#include "apis/windows/kl_windows.h"

// DirectX
#include <wrl.h>
#include <gdiplus.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

namespace kl::dx {
    /* Types */
    // Device
    using Device = Microsoft::WRL::ComPtr<ID3D11Device>;
    using Context = Microsoft::WRL::ComPtr<ID3D11DeviceContext>;
    using Chain = Microsoft::WRL::ComPtr<IDXGISwapChain>;

    // Data
    using Buffer = Microsoft::WRL::ComPtr<ID3D11Buffer>;
    using Texture = Microsoft::WRL::ComPtr<ID3D11Texture2D>;
    using Resource = Microsoft::WRL::ComPtr<ID3D11Resource>;

    // State
    using RasterState = Microsoft::WRL::ComPtr<ID3D11RasterizerState>;
    using DepthState = Microsoft::WRL::ComPtr<ID3D11DepthStencilState>;
    using SamplerState = Microsoft::WRL::ComPtr<ID3D11SamplerState>;
    using BlendState = Microsoft::WRL::ComPtr<ID3D11BlendState>;

    // View
    using TargetView = Microsoft::WRL::ComPtr<ID3D11RenderTargetView>;
    using DepthView = Microsoft::WRL::ComPtr<ID3D11DepthStencilView>;
    using ShaderView = Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>;
    using AccessView = Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>;

    // Shader data
    using DataBlob = Microsoft::WRL::ComPtr<ID3DBlob>;
    using InputLayout = Microsoft::WRL::ComPtr<ID3D11InputLayout>;

    // Shaders
    using VertexShader = Microsoft::WRL::ComPtr<ID3D11VertexShader>;
    using PixelShader = Microsoft::WRL::ComPtr<ID3D11PixelShader>;
    using GeometryShader = Microsoft::WRL::ComPtr<ID3D11GeometryShader>;
    using HullShader = Microsoft::WRL::ComPtr<ID3D11HullShader>;
    using DomainShader = Microsoft::WRL::ComPtr<ID3D11DomainShader>;
    using ComputeShader = Microsoft::WRL::ComPtr<ID3D11ComputeShader>;

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
