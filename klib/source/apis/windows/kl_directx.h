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
    using device = Microsoft::WRL::ComPtr<ID3D11Device>;
    using context = Microsoft::WRL::ComPtr<ID3D11DeviceContext>;
    using chain = Microsoft::WRL::ComPtr<IDXGISwapChain>;

    // Data
    using buffer = Microsoft::WRL::ComPtr<ID3D11Buffer>;
    using texture = Microsoft::WRL::ComPtr<ID3D11Texture2D>;
    using resource = Microsoft::WRL::ComPtr<ID3D11Resource>;

    // State
    using raster_state = Microsoft::WRL::ComPtr<ID3D11RasterizerState>;
    using depth_state = Microsoft::WRL::ComPtr<ID3D11DepthStencilState>;
    using sampler_state = Microsoft::WRL::ComPtr<ID3D11SamplerState>;
    using blend_state = Microsoft::WRL::ComPtr<ID3D11BlendState>;

    // View
    using target_view = Microsoft::WRL::ComPtr<ID3D11RenderTargetView>;
    using depth_view = Microsoft::WRL::ComPtr<ID3D11DepthStencilView>;
    using shader_view = Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>;
    using access_view = Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>;

    // Shader data
    using data_blob = Microsoft::WRL::ComPtr<ID3DBlob>;
    using layout = Microsoft::WRL::ComPtr<ID3D11InputLayout>;

    // Shaders
    using vertex_shader = Microsoft::WRL::ComPtr<ID3D11VertexShader>;
    using pixel_shader = Microsoft::WRL::ComPtr<ID3D11PixelShader>;
    using geometry_shader = Microsoft::WRL::ComPtr<ID3D11GeometryShader>;
    using hull_shader = Microsoft::WRL::ComPtr<ID3D11HullShader>;
    using domain_shader = Microsoft::WRL::ComPtr<ID3D11DomainShader>;
    using compute_shader = Microsoft::WRL::ComPtr<ID3D11ComputeShader>;

    /* Descriptors */
    // Data
    using buffer_descriptor = D3D11_BUFFER_DESC;
    using texture_descriptor = D3D11_TEXTURE2D_DESC;
    using subresource_descriptor = D3D11_SUBRESOURCE_DATA;
    using mapped_subresource_descriptor = D3D11_MAPPED_SUBRESOURCE;

    // Input
    using layout_descriptor = D3D11_INPUT_ELEMENT_DESC;

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
}
