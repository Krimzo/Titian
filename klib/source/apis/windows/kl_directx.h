#pragma once

// Windows
#include "apis/windows/kl_windows.h"

// DirectX
#include <wrl.h>
#include <gdiplus.h>
#include <d3dcompiler.h>
#include <d3d11.h>

#pragma comment(lib, "gdiplus.lib")
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "d3d11.lib")

// DxHelper
using namespace Microsoft::WRL;

namespace kl::dx {
    /* Types */
    // Device
    using device = ComPtr<ID3D11Device>;
    using context = ComPtr<ID3D11DeviceContext>;
    using chain = ComPtr<IDXGISwapChain>;

    // Data
    using buffer = ComPtr<ID3D11Buffer>;
    using texture = ComPtr<ID3D11Texture2D>;
    using resource = ComPtr<ID3D11Resource>;

    // State
    using raster_state = ComPtr<ID3D11RasterizerState>;
    using depth_state = ComPtr<ID3D11DepthStencilState>;
    using sampler_state = ComPtr<ID3D11SamplerState>;
    using blend_state = ComPtr<ID3D11BlendState>;

    // View
    using target_view = ComPtr<ID3D11RenderTargetView>;
    using depth_view = ComPtr<ID3D11DepthStencilView>;
    using shader_view = ComPtr<ID3D11ShaderResourceView>;
    using access_view = ComPtr<ID3D11UnorderedAccessView>;

    // Shader data
    using shader_blob = ComPtr<ID3DBlob>;
    using layout = ComPtr<ID3D11InputLayout>;

    // Shaders
    using vertex_shader = ComPtr<ID3D11VertexShader>;
    using pixel_shader = ComPtr<ID3D11PixelShader>;
    using geometry_shader = ComPtr<ID3D11GeometryShader>;
    using hull_shader = ComPtr<ID3D11HullShader>;
    using domain_shader = ComPtr<ID3D11DomainShader>;
    using compute_shader = ComPtr<ID3D11ComputeShader>;

    /* Descriptors */
    // Data
    using buffer_descriptor = D3D11_BUFFER_DESC;
    using texture_descriptor = D3D11_TEXTURE2D_DESC;
    using subresource_descriptor = D3D11_SUBRESOURCE_DATA;
    using mapped_subresource_descriptor = D3D11_MAPPED_SUBRESOURCE;

    // State
    using raster_state_descriptor = D3D11_RASTERIZER_DESC;
    using depth_state_descriptor = D3D11_DEPTH_STENCIL_DESC;
    using sampler_state_descriptor = D3D11_SAMPLER_DESC;
    using blend_state_descriptor = D3D11_BLEND_DESC;

    // View
    using target_view_descriptor = D3D11_RENDER_TARGET_VIEW_DESC;
    using depth_view_descriptor = D3D11_DEPTH_STENCIL_VIEW_DESC;
    using shader_view_descriptor = D3D11_SHADER_RESOURCE_VIEW_DESC;
    using access_view_descriptor = D3D11_UNORDERED_ACCESS_VIEW_DESC;

    // Shader
    using layout_descriptor = D3D11_INPUT_ELEMENT_DESC;
}
