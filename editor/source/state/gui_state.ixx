export module gui_state;

export import basic_state;

export struct gui_state : public BasicState
{
    bool is_viewport_focused = false;
    bool render_collider = true;

    int gizmo_operation = 0;
    int gizmo_mode = 1;
    int explorer_icon_size = 65;

    kl::int2 viewport_size = {};

    kl::float4 color_special = kl::color(235, 170, 15);
    kl::float4 color_alternative = kl::color(127, 190, 190);

    std::string builtin_path = "./builtin/";
    std::string explorer_path = std::filesystem::absolute(".").string();

    struct textures_t
    {
        kl::object<kl::texture> folder = {};
        kl::object<kl::texture> folder_empty = {};
        kl::object<kl::texture> folder_parent = {};
        kl::object<kl::texture> file = {};
        kl::object<kl::texture> code = {};
        kl::object<kl::texture> script = {};
        kl::object<kl::texture> mesh = {};
        kl::object<kl::texture> texture = {};
        kl::object<kl::texture> scene = {};
    } textures;

    // Mesh editor
    struct mesh_editor_t
    {
        kl::camera camera = {};
        kl::object<kl::mesh> selected_mesh = {};
        kl::object<kl::texture> render_texture = {};
        kl::render_shaders shaders = {};

        mesh_editor_t()
        {
            this->camera.origin = { 0.642787576f, 0.577350259f, 0.766044438f };
            this->camera.speed = 3.1f;
        }
    } mesh_editor;

    // Material editor
    struct material_editor_t
    {
        kl::camera camera = {};
        kl::object<kl::material> selected_material = {};
        kl::object<kl::texture> selected_texture = {};
        kl::object<kl::texture> render_texture = {};
        kl::render_shaders shaders = {};

        material_editor_t()
        {
            this->camera.origin = { 0.642787576f, 0.577350259f, 0.766044438f };
            this->camera.speed = 3.1f;
        }
    } material_editor;

    // Render info
    struct render_info_t
    {
        float last_update_time = 0.0f;
    } render_info;

    gui_state(kl::object<kl::gpu> gpu)
    {
        auto load_texture = [&](const std::string& path)
        {
            kl::object<kl::texture> texture = new kl::texture(&gpu, kl::image(path));
            texture->create_shader_view();
            return texture;
        };

        textures.folder = load_texture(builtin_path + "textures/folder.png");
        textures.folder_empty = load_texture(builtin_path + "textures/folder_empty.png");
        textures.folder_parent = load_texture(builtin_path + "textures/folder_parent.png");
        textures.file = load_texture(builtin_path + "textures/file.png");
        textures.code = load_texture(builtin_path + "textures/code.png");
        textures.script = load_texture(builtin_path + "textures/script.png");
        textures.mesh = load_texture(builtin_path + "textures/mesh.png");
        textures.texture = load_texture(builtin_path + "textures/texture.png");
        textures.scene = load_texture(builtin_path + "textures/scene.png");
    }

    gui_state(gui_state&) = delete;
    gui_state(gui_state&&) = delete;

    void operator=(gui_state&) = delete;
    void operator=(gui_state&&) = delete;
};
