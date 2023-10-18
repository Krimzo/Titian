export module shader_states;

export import klib;

export namespace titian {
    class ShaderStates
    {
    public:
        kl::RenderShaders shadow_pass = {};
        kl::RenderShaders skybox_pass = {};
        kl::RenderShaders material_pass = {};
        kl::RenderShaders unlit_pass = {};
        kl::RenderShaders lit_pass = {};
        kl::RenderShaders postprocess_pass = {};
        
        ShaderStates(kl::Object<kl::GPU>& gpu)
        {
            const auto parse_render_shaders = [&](const std::string& name)
            {
                const std::string source = kl::read_file_string("builtin/shaders/" + name);
                return gpu->create_render_shaders(source);
            };
            
            shadow_pass = parse_render_shaders("shadow_pass.hlsl");
            skybox_pass = parse_render_shaders("skybox_pass.hlsl");
            material_pass = parse_render_shaders("material_pass.hlsl");
            unlit_pass = parse_render_shaders("unlit_pass.hlsl");
            lit_pass = parse_render_shaders("lit_pass.hlsl");
            postprocess_pass = parse_render_shaders("postprocess_pass.hlsl");

            kl::assert(shadow_pass, "Failed to init SHADOW_PASS render shaders.");
            kl::assert(skybox_pass, "Failed to init SKYBOX_PASS render shaders.");
            kl::assert(material_pass, "Failed to init MATERIAL_PASS render shaders.");
            kl::assert(unlit_pass, "Failed to init UNLIT_PASS render shaders.");
            kl::assert(lit_pass, "Failed to init LIT_PASS render shaders.");
            kl::assert(postprocess_pass, "Failed to init POSTPROCESS_PASS render shaders.");
        }
    };
}
