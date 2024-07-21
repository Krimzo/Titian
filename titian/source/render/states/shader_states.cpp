#include "titian.h"


titian::ShaderStates::ShaderStates(kl::GPU* gpu)
{
    auto load_shader = [&](kl::RenderShaders& shader, const char* filename, const Vector<dx::LayoutDescriptor>& layout_descriptors)
	{
        const String source = kl::read_file_string(kl::format("package/shaders/", filename));
        shader = gpu->create_render_shaders(source, layout_descriptors);
        kl::assert(shader, "Failed to init [", filename, "] shaders");
	};

    const Vector<dx::LayoutDescriptor> vertex_layout_descriptors = {
        {    "KL_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        {     "KL_Texture", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        {      "KL_Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "KL_BoneIndices", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "KL_BoneWeights", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    WorkQueue queue;
    queue.add_task([&] { load_shader(solid_pass, "solid_pass.hlsl", {}); });
    queue.add_task([&] { load_shader(solid_lit_pass, "solid_lit_pass.hlsl", vertex_layout_descriptors); });
    queue.add_task([&] { load_shader(shadow_pass, "shadow_pass.hlsl", vertex_layout_descriptors); });
    queue.add_task([&] { load_shader(skybox_pass, "skybox_pass.hlsl", vertex_layout_descriptors); });
    queue.add_task([&] { load_shader(scene_pass, "scene_pass.hlsl", vertex_layout_descriptors); });
    queue.add_task([&] { load_shader(post_pass, "post_pass.hlsl", {}); });
    queue.add_task([&] { load_shader(outline_pass, "outline_pass.hlsl", {}); });
    queue.add_task([&] { load_shader(display_pass, "display_pass.hlsl", {}); });
    queue.finalize();
}
