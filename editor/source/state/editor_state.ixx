export module editor_state;

export import logger_state;
export import render_state;
export import gui_state;

export import basic_script;
export import jdk;

export inline const std::string builtin_path = "./builtin/";

export struct editor_state : public BasicState
{
    // System
    kl::timer timer = {};
    kl::object<kl::window> window = new kl::window("Titian", { 1600, 900 });
    kl::object<kl::gpu> gpu = new kl::gpu((HWND) *window,
#ifdef NDEBUG
        false
#else
        true
#endif
    );

    // Render features
    bool game_running = false;
    bool render_wireframe = false;
    bool v_sync = true;

    // Graphics states
    struct raster_states_t
    {
        kl::dx::raster_state wireframe = nullptr;
        kl::dx::raster_state solid_cull = nullptr;
        kl::dx::raster_state solid = nullptr;
        kl::dx::raster_state shadow = nullptr;
    } raster_states = {};

    struct depth_states_t
    {
        kl::dx::depth_state disabled = nullptr;
        kl::dx::depth_state enabled = nullptr;
    } depth_states = {};

    struct sampler_states_t
    {
        kl::dx::sampler_state linear = nullptr;
        kl::dx::sampler_state shadow = nullptr;
    } sampler_states = {};

    // Shaders
    struct render_shaders_t
    {
        kl::render_shaders shadow_pass = {};
        kl::render_shaders skybox_sample = {};
        kl::render_shaders object_single = {};
        kl::render_shaders object_material = {};
        kl::render_shaders object_full = {};
        kl::render_shaders postprocess_pass = {};
    } render_shaders;

    // Scene components
    struct default_mesh_t
    {
        kl::object<kl::mesh> cube = {};
        kl::object<kl::mesh> sphere = {};
        kl::object<kl::mesh> capsule = {};
        kl::object<kl::mesh> monke = {};
    } default_mesh = {};
    struct default_material_t
    {
        kl::object<kl::material> white = {};
    } default_material = {};
    kl::object<kl::scene> scene = new kl::scene();

    // Scripts
    std::map<std::string, kl::object<basic_script>> scripts = {};

    // Engine states
    kl::object<logger_state> logger_state = new ::logger_state();
    kl::object<render_state> render_state = new ::render_state(gpu, window->size());
    kl::object<gui_state> gui_state = new ::gui_state(gpu);

    editor_state()
    {
        window->set_icon(builtin_path + "textures/titian.ico");
        change_scene(new kl::scene());
    }

    virtual ~editor_state()
    {
        gui_state = nullptr;
        render_state = nullptr;
        logger_state = nullptr;
        change_scene(nullptr);
    }

    editor_state(editor_state&) = delete;
    editor_state(editor_state&&) = delete;

    void operator=(editor_state&) = delete;
    void operator=(editor_state&&) = delete;

    void change_scene(kl::object<kl::scene> scene)
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
        default_mesh.cube = new kl::mesh(&gpu, &scene, kl::parse_obj_file(builtin_path + "meshes/cube.obj", true));
        default_mesh.sphere = new kl::mesh(&gpu, &scene, kl::parse_obj_file(builtin_path + "meshes/sphere.obj", true));
        default_mesh.capsule = new kl::mesh(&gpu, &scene, kl::parse_obj_file(builtin_path + "meshes/capsule.obj", true));
        default_mesh.monke = new kl::mesh(&gpu, &scene, kl::parse_obj_file(builtin_path + "meshes/monke.obj", true));

        // Default material
        default_material.white = new kl::material();
        default_material.white->color = kl::colors::white;
    }

    void scripts_update_static_info(editor_state* state)
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

    void scripts_call_start(editor_state* state)
    {
        for (auto& [_, script] : scripts) {
            script->call_start(state);
        }
    }

    void scripts_call_update(editor_state* state)
    {
        for (auto& [_, script] : scripts) {
            script->call_update(state);
        }
    }
};
