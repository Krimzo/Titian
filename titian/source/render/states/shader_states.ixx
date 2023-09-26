export module shader_states;

export import klib;

export namespace titian {
    class ShaderStates
    {
    public:
        kl::RenderShaders shadow_pass = {};
        kl::RenderShaders skybox_sample = {};
        kl::RenderShaders object_single = {};
        kl::RenderShaders object_material = {};
        kl::RenderShaders object_full = {};
        kl::RenderShaders postprocess_pass = {};
        
        ShaderStates(kl::GPU* gpu)
        {
            auto parse_render_shaders = [&](const std::string& name)
            {
                const std::string source = kl::read_file_string("builtin/shaders/" + name);
                return gpu->create_render_shaders(source);
            };
            
            shadow_pass = parse_render_shaders("shadow_pass.hlsl");
            skybox_sample = parse_render_shaders("skybox_sample.hlsl");
            object_single = parse_render_shaders("object_single.hlsl");
            object_material = parse_render_shaders("object_material.hlsl");
            object_full = parse_render_shaders("object_full.hlsl");
            postprocess_pass = parse_render_shaders("postprocess_pass.hlsl");

            kl::assert(shadow_pass, "Failed to init SHADOW_PASS render shaders.");
            kl::assert(skybox_sample, "Failed to init SKYBOX_SAMPLE render shaders.");
            kl::assert(object_single, "Failed to init OBJECT_SINGLE render shaders.");
            kl::assert(object_material, "Failed to init OBJECT_MATERIAL render shaders.");
            kl::assert(object_full, "Failed to init OBJECT_FULL render shaders.");
            kl::assert(postprocess_pass, "Failed to init POSTPROCESS_PASS render shaders.");
        }
    };
}
