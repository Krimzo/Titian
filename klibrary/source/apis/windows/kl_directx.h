#pragma once

// Other
#include "apis/windows/kl_windows.h"
#include "apis/external/kl_dx12.h"

// DirectX
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
    template<typename T>
	    requires std::is_base_of_v<IUnknown, T>
    struct ComRef
    {
        // create
        inline ComRef()
        {}

        inline explicit ComRef(T* instance)
            : m_instance(instance)
        {}

        // destroy
        inline ~ComRef() noexcept
        {
            this->free();
        }

        inline void free()
        {
            if (m_instance) {
                m_instance->Release();
                m_instance = nullptr;
            }
        }

        // copy
        inline ComRef(const ComRef& other)
            : m_instance(other.m_instance)
        {
            this->increase_count();
        }

        inline ComRef& operator=(const ComRef& other)
        {
            if (other.m_instance != m_instance) {
                this->free();
                m_instance = other.m_instance;
                this->increase_count();
            }
            return *this;
        }

        // move
        inline ComRef(ComRef&& other) noexcept
        {
            *this = other;
            other.free();
        }

        inline ComRef& operator=(ComRef&& other) noexcept
        {
            *this = other;
            other.free();
            return *this;
        }

        // cast 
        template<typename B>
            requires std::is_base_of_v<B, T>
        inline operator ComRef<B>() const
        {
            this->increase_count();
            return ComRef<B>{m_instance};
        }

        template<typename U>
        inline HRESULT as(ComRef<U>& out) const
        {
            return m_instance->QueryInterface(__uuidof(U), (void**) &out);
        }

        // access
        inline T** operator&()
        {
            this->free();
            return &m_instance;
        }

        inline T& operator*() const
        {
            return *m_instance;
        }
        
        inline T* operator->() const
        {
            return m_instance;
        }

        inline T* get() const
        {
			return m_instance;
        }

        inline T** address()
        {
            return &m_instance;
        }

        inline T*const* address() const
        {
            return &m_instance;
        }

        // info
        inline operator bool() const
        {
            return static_cast<bool>(m_instance);
        }

    private:
        T* m_instance = nullptr;

        inline void increase_count() const
        {
            if (m_instance) {
                m_instance->AddRef();
            }
        }
    };
}

namespace kl {
    using TextFormat = ComRef<IDWriteTextFormat>;
}

namespace kl::dx {
    /* Types */
    // Device
    using Device = ComRef<ID3D11Device4>;
    using Context = ComRef<ID3D11DeviceContext4>;
    using Chain = ComRef<IDXGISwapChain4>;

    // Data
    using Buffer = ComRef<ID3D11Buffer>;
    using Texture = ComRef<ID3D11Texture2D>;
    using Resource = ComRef<ID3D11Resource>;

    // State
    using RasterState = ComRef<ID3D11RasterizerState>;
    using DepthState = ComRef<ID3D11DepthStencilState>;
    using SamplerState = ComRef<ID3D11SamplerState>;
    using BlendState = ComRef<ID3D11BlendState>;

    // View
    using TargetView = ComRef<ID3D11RenderTargetView>;
    using DepthView = ComRef<ID3D11DepthStencilView>;
    using ShaderView = ComRef<ID3D11ShaderResourceView>;
    using AccessView = ComRef<ID3D11UnorderedAccessView>;

    // Shader data
    using DataBlob = ComRef<ID3DBlob>;
    using InputLayout = ComRef<ID3D11InputLayout>;

    // Shaders
    using VertexShader = ComRef<ID3D11VertexShader>;
    using PixelShader = ComRef<ID3D11PixelShader>;
    using GeometryShader = ComRef<ID3D11GeometryShader>;
    using HullShader = ComRef<ID3D11HullShader>;
    using DomainShader = ComRef<ID3D11DomainShader>;
    using ComputeShader = ComRef<ID3D11ComputeShader>;

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
    using Device = ComRef<ID3D12Device8>;

    // Context commands
    using CommandQueue = ComRef<ID3D12CommandQueue>;
    using CommandAllocator = ComRef<ID3D12CommandAllocator>;
    using CommandList = ComRef<ID3D12GraphicsCommandList4>;
    using Fence = ComRef<ID3D12Fence>;

    // Swapchain
    using DXGIFactory = ComRef<IDXGIFactory4>;
    using SwapChain = ComRef<IDXGISwapChain4>;

    // Resources
    using DescriptorHeap = ComRef<ID3D12DescriptorHeap>;
    using DescriptorHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE;
    using Resource = ComRef<ID3D12Resource>;
    using VertexBuffer = D3D12_VERTEX_BUFFER_VIEW;

    // Pipeline stuff
    using Blob = ComRef<ID3DBlob>;
    using ShaderByteCode = CD3DX12_SHADER_BYTECODE;
    using InputLayout = D3D12_INPUT_ELEMENT_DESC;
    using RootSignature = ComRef<ID3D12RootSignature>;
    using PipelineState = ComRef<ID3D12PipelineState>;
    using StateObject = ComRef<ID3D12StateObject>;

    // Clippers
    using Scissors = CD3DX12_RECT;
    using Viewport = CD3DX12_VIEWPORT;

    // Raytracing
    using AccelerationInputs = D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS;
    using AccelerationStructure = ComRef<ID3D12Resource>;
    using ObjectProperties = ComRef<ID3D12StateObjectProperties>;
}
