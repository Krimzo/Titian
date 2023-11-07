#pragma once

#include "klibrary.h"


namespace titian {
    class ShaderStates
    {
    public:
        kl::RenderShaders shadow_pass = {};
        kl::RenderShaders skybox_pass = {};
        kl::RenderShaders material_pass = {};
        kl::RenderShaders unlit_pass = {};
        kl::RenderShaders lit_pass = {};
        kl::RenderShaders pp_pass = {};
        kl::RenderShaders display_pass = {};

        ShaderStates(kl::GPU* gpu);
    };
}
