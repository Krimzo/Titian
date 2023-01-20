#pragma once

#include "window/system_handler.h"


namespace kl::dx {
    /* Types */
    // Device
    using device = ID3D11Device*;
    using context = ID3D11DeviceContext*;
    using chain = IDXGISwapChain*;

    // Data
    using buffer = ID3D11Buffer*;
    using texture = ID3D11Texture2D*;
    using resource = ID3D11Resource*;

    // State
    using raster_state = ID3D11RasterizerState*;
    using depth_state = ID3D11DepthStencilState*;
    using sampler_state = ID3D11SamplerState*;
    using blend_state = ID3D11BlendState*;

    // View
    using target_view = ID3D11RenderTargetView*;
    using depth_view = ID3D11DepthStencilView*;
    using shader_view = ID3D11ShaderResourceView*;
    using access_view = ID3D11UnorderedAccessView*;

    // Shader
    using vertex_shader = ID3D11VertexShader*;
    using pixel_shader = ID3D11PixelShader*;
    using geometry_shader = ID3D11GeometryShader*;
    using hull_shader = ID3D11HullShader*;
    using domain_shader = ID3D11DomainShader*;
    using compute_shader = ID3D11ComputeShader*;
    using layout = ID3D11InputLayout*;

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
