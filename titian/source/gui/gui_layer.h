#pragma once

#include "render/render_layer.h"
#include "gui/gui_section.h"


namespace titian
{
struct GUILayer : Layer_T<GUILayer>
{
    Vector<Ref<GUISection>> sections;
    float dpi_scaling = 1.0f;

    Float4 special_color = kl::colors::WHITE;
    Float4 alternate_color = kl::colors::BLACK;

    ImFont* roboto_font_small = nullptr;
    ImFont* roboto_font_large = nullptr;

    GUILayer();

    bool update() override;

    void reload_fonts();
    void reload_colors();
};
}
