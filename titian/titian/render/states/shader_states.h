#pragma once

#include "klibrary.h"


namespace titian {
    class ShaderStates
    {
    public:
        kl::RenderShaders solid_pass = {};
        kl::RenderShaders solid_lit_pass = {};

        kl::RenderShaders shadow_pass = {};
        kl::RenderShaders skybox_pass = {};
        kl::RenderShaders scene_pass = {};
        kl::RenderShaders outline_pass = {};
        kl::RenderShaders display_pass = {};

        ShaderStates(kl::GPU* gpu);
    };
}
