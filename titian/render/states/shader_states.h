#pragma once

#include "standard.h"


namespace titian
{
struct ShaderStates
{
    kl::Shaders solid_pass;
    kl::Shaders solid_lit_pass;

    kl::Shaders shadow_pass;
    kl::Shaders skybox_pass;
    kl::Shaders scene_pass;
    kl::Shaders post_pass;
    kl::Shaders outline_pass;
    kl::Shaders display_pass;

    ShaderStates();
};
}
