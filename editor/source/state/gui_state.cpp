#include "editor.h"


kl::object<kl::texture> load_texture(kl::object<kl::gpu> gpu, const std::string& path);

gui_state::gui_state(kl::object<kl::gpu> gpu)
{
    textures.folder = load_texture(gpu, builtin_path + "textures/folder.png");
    textures.folder_empty = load_texture(gpu, builtin_path + "textures/folder_empty.png");
    textures.folder_parent = load_texture(gpu, builtin_path + "textures/folder_parent.png");
    textures.file = load_texture(gpu, builtin_path + "textures/file.png");
    textures.code = load_texture(gpu, builtin_path + "textures/code.png");
    textures.script = load_texture(gpu, builtin_path + "textures/script.png");
    textures.mesh = load_texture(gpu, builtin_path + "textures/mesh.png");
    textures.texture = load_texture(gpu, builtin_path + "textures/texture.png");
    textures.scene = load_texture(gpu, builtin_path + "textures/scene.png");
}

kl::object<kl::texture> load_texture(kl::object<kl::gpu> gpu, const std::string& path)
{
    kl::object<kl::texture> texture = new kl::texture(&gpu, kl::image(path));
    texture->create_shader_view();
    return texture;
}
