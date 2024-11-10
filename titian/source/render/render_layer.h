#pragma once

#include "editor/game_layer.h"
#include "render/render_pass.h"
#include "scene/camera.h"

#include "render/states/raster_states.h"
#include "render/states/depth_states.h"
#include "render/states/sampler_states.h"
#include "render/states/shader_states.h"
#include "render/states/blend_states.h"


namespace titian
{
struct RenderLayer : Layer_T<RenderLayer>
{
    Vector<Ref<RenderPass>> passes;

    RasterStates raster_states;
    DepthStates depth_states;
    SamplerStates sampler_states;
    ShaderStates shader_states;
    BlendStates blend_states;

    dx::Buffer screen_mesh;

    RenderLayer();

    bool update() override;
    void present() const;
};
}
