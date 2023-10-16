export module gui_state;

export import basic_state;

export class GUIState : public BasicState
{
public:
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
};
