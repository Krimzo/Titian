export module editor_state;

export import logger_state;
export import render_state;
export import gui_state;

export import basic_script;
export import jdk;

export inline const std::string BUILTIN_PATH = "./builtin/";

export class EditorState : public BasicState
{
public:
    // Render features
    bool game_running = false;
    bool render_wireframe = false;
    bool v_sync = true;

    // Scene components
    class DefaultMeshT
    {
    public:
        kl::Object<kl::Mesh> cube = {};
        kl::Object<kl::Mesh> sphere = {};
        kl::Object<kl::Mesh> capsule = {};
        kl::Object<kl::Mesh> monke = {};
    } default_mesh = {};
    
    class DefaultMaterialT
    {
    public:
        kl::Object<kl::Material> white = {};
    } default_material = {};
    
    kl::Object<kl::Scene> scene = new kl::Scene();

    // Scripts
    std::map<std::string, kl::Object<BasicScript>> scripts = {};

    // Engine states
    kl::Object<LoggerState> logger_state = new ::LoggerState();
    kl::Object<RenderState> render_state = new ::RenderState(gpu, window->size());
    kl::Object<GUIState> gui_state = new ::GUIState(gpu);

    EditorState()
    {
        window->set_icon(BUILTIN_PATH + "textures/titian.ico");
        change_scene(new kl::Scene());
    }

    virtual ~EditorState() override
    {
        gui_state = nullptr;
        render_state = nullptr;
        logger_state = nullptr;
        change_scene(nullptr);
    }

    EditorState(EditorState&) = delete;
    EditorState(EditorState&&) = delete;

    void operator=(EditorState&) = delete;
    void operator=(EditorState&&) = delete;

    void change_scene(kl::Object<kl::Scene> scene)
    {
        default_mesh.cube = nullptr;
        default_mesh.sphere = nullptr;
        default_mesh.capsule = nullptr;
        default_mesh.monke = nullptr;
        default_material.white = nullptr;

        if (gui_state) {
            gui_state->mesh_editor.selected_mesh = nullptr;
            gui_state->material_editor.selected_material = nullptr;
            gui_state->material_editor.selected_texture = nullptr;
        }

        this->scene = scene;
        if (!scene) {
            return;
        }

        // Default meshes
        default_mesh.cube = new kl::Mesh(&gpu, &scene, kl::parse_obj_file(BUILTIN_PATH + "meshes/cube.obj", true));
        default_mesh.sphere = new kl::Mesh(&gpu, &scene, kl::parse_obj_file(BUILTIN_PATH + "meshes/sphere.obj", true));
        default_mesh.capsule = new kl::Mesh(&gpu, &scene, kl::parse_obj_file(BUILTIN_PATH + "meshes/capsule.obj", true));
        default_mesh.monke = new kl::Mesh(&gpu, &scene, kl::parse_obj_file(BUILTIN_PATH + "meshes/monke.obj", true));

        // Default material
        default_material.white = new kl::Material();
        default_material.white->color = kl::colors::WHITE;
    }

    void scripts_update_static_info(const EditorState* state)
    {
        static jclass class_engine = java::load_eternal_class("Lsdk/engine/Engine;");
        if (!class_engine) {
            return;
        }

        static jfield field_elapsed_t = java::jni->GetStaticFieldID(class_engine, "elapsedTime", signatures::FLOAT.c_str());
        if (!field_elapsed_t) {
            return;
        }

        static jfield field_delta_t = java::jni->GetStaticFieldID(class_engine, "deltaTime", signatures::FLOAT.c_str());
        if (!field_delta_t) {
            return;
        }

        const float elapsed_t = state->timer.elapsed();
        const float delta_t = state->timer.delta();

        java::jni->SetStaticFloatField(class_engine, field_elapsed_t, elapsed_t);
        java::jni->SetStaticFloatField(class_engine, field_delta_t, delta_t);
    }

    void scripts_call_start(EditorState* state)
    {
        for (auto& [_, script] : scripts) {
            script->call_start(state);
        }
    }

    void scripts_call_update(EditorState* state)
    {
        for (auto& [_, script] : scripts) {
            script->call_update(state);
        }
    }
};
