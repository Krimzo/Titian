export module gui_state;

export import basic_state;

export class GUIState : public BasicState
{
public:
    bool is_viewport_focused = false;

    class TexturesT
    {
    public:
        kl::Object<kl::Texture> folder = {};
        kl::Object<kl::Texture> folder_empty = {};
        kl::Object<kl::Texture> folder_parent = {};
        kl::Object<kl::Texture> file = {};
        kl::Object<kl::Texture> code = {};
        kl::Object<kl::Texture> script = {};
        kl::Object<kl::Texture> mesh = {};
        kl::Object<kl::Texture> texture = {};
        kl::Object<kl::Texture> scene = {};
    } textures;

    // Mesh editor
    class MeshEditorT
    {
    public:
        kl::Camera camera = {};
        kl::Object<kl::Mesh> selected_mesh = {};
        kl::Object<kl::Texture> render_texture = {};
        kl::RenderShaders shaders = {};

        MeshEditorT()
        {
            this->camera.origin = { 0.642787576f, 0.577350259f, 0.766044438f };
            this->camera.speed = 3.1f;
        }
    } mesh_editor;

    // Material editor
    class MaterialEditorT
    {
    public:
        kl::Camera camera = {};
        kl::Object<kl::Material> selected_material = {};
        kl::Object<kl::Texture> selected_texture = {};
        kl::Object<kl::Texture> render_texture = {};
        kl::RenderShaders shaders = {};

        MaterialEditorT()
        {
            this->camera.origin = { 0.642787576f, 0.577350259f, 0.766044438f };
            this->camera.speed = 3.1f;
        }
    } material_editor;

    // Render info
    class RenderInfoT
    {
    public:
        float last_update_time = 0.0f;
    } render_info;

    GUIState(kl::Object<kl::GPU> gpu)
    {
        auto load_texture = [&](const std::string& path)
        {
            kl::Object texture = new kl::Texture(&gpu, kl::Image(path));
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

    GUIState(GUIState&) = delete;
    GUIState(GUIState&&) = delete;

    void operator=(GUIState&) = delete;
    void operator=(GUIState&&) = delete;
};
