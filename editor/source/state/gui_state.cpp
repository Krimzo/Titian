#include "state/gui_state.h"


kl::ref<kl::texture> load_texture(kl::ref<kl::gpu> gpu, const std::string& path);

gui_state::gui_state(kl::ref<kl::gpu> gpu)
{
    textures["folder"] = load_texture(gpu, builtin_path + "textures/folder.png");
    textures["folder_empty"] = load_texture(gpu, builtin_path + "textures/folder_empty.png");
    textures["file"] = load_texture(gpu, builtin_path + "textures/file.png");
    textures["code"] = load_texture(gpu, builtin_path + "textures/code.png");
    textures["script"] = load_texture(gpu, builtin_path + "textures/script.png");
    textures["mesh"] = load_texture(gpu, builtin_path + "textures/mesh.png");
    textures["image"] = load_texture(gpu, builtin_path + "textures/image.png");
    textures["scene"] = load_texture(gpu, builtin_path + "textures/scene.png");
}

gui_state::~gui_state()
{}

kl::ref<kl::texture> load_texture(kl::ref<kl::gpu> gpu, const std::string& path)
{
    auto texture = kl::make<kl::texture>(gpu.get(), kl::image(path));
    texture->create_shader_view();
    return texture;
}
