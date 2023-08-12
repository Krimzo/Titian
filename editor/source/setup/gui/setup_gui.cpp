#include "editor.h"


void load_custom_theme(editor_state* state);
void load_custom_font(const editor_state* state);

void setup_gui(editor_state* state)
{
    ImGui::CreateContext();
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();
    load_custom_theme(state);
    load_custom_font(state);

    ImGui_ImplWin32_Init(*state->window);
    ImGui_ImplDX11_Init(state->gpu->device().Get(), state->gpu->context().Get());
}

void load_custom_theme(editor_state* state)
{
    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowPadding = ImVec2(15.0f, 15.0f);
    style.WindowRounding = 2.0f;
    style.FramePadding = ImVec2(5.0f, 5.0f);
    style.FrameRounding = 2.0f;
    style.ItemSpacing = ImVec2(12.0f, 8.0f);
    style.ItemInnerSpacing = ImVec2(8.0f, 6.0f);
    style.SelectableTextAlign = ImVec2(0.5f, 0.5f);
    style.IndentSpacing = 25.0f;
    style.ScrollbarSize = 15.0f;
    style.ScrollbarRounding = 9.0f;
    style.GrabMinSize = 5.0f;
    style.GrabRounding = 3.0f;
    style.PopupBorderSize = 1.0f;
    style.PopupRounding = 5.0f;
    style.ChildBorderSize = 1.0f;
    style.ChildRounding = 5.0f;
    
    constexpr auto color_dark = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    constexpr auto color_mid = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
    constexpr auto color_light = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
    constexpr auto color_spec = ImVec4(0.7f, 0.4f, 0.0f, 1.0f);

    style.Colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.0f);
    style.Colors[ImGuiCol_TextDisabled] = color_light;
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.077f, 0.077f, 0.077f, 1.0f);
    style.Colors[ImGuiCol_ChildBg] = color_dark;
    style.Colors[ImGuiCol_PopupBg] = color_dark;
    style.Colors[ImGuiCol_Border] = color_light;
    style.Colors[ImGuiCol_BorderShadow] = color_mid;
    style.Colors[ImGuiCol_FrameBg] = color_dark;
    style.Colors[ImGuiCol_FrameBgHovered] = color_mid;
    style.Colors[ImGuiCol_FrameBgActive] = color_light;
    style.Colors[ImGuiCol_TitleBg] = color_dark;
    style.Colors[ImGuiCol_TitleBgActive] = color_dark;
    style.Colors[ImGuiCol_TitleBgCollapsed] = color_dark;
    style.Colors[ImGuiCol_MenuBarBg] = color_dark;
    style.Colors[ImGuiCol_ScrollbarBg] = color_dark;
    style.Colors[ImGuiCol_ScrollbarGrab] = color_light;
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = color_mid;
    style.Colors[ImGuiCol_ScrollbarGrabActive] = color_light;
    style.Colors[ImGuiCol_CheckMark] = color_spec;
    style.Colors[ImGuiCol_SliderGrab] = color_spec;
    style.Colors[ImGuiCol_SliderGrabActive] = color_spec;
    style.Colors[ImGuiCol_Button] = color_mid;
    style.Colors[ImGuiCol_ButtonHovered] = color_light;
    style.Colors[ImGuiCol_ButtonActive] = color_light;
    style.Colors[ImGuiCol_Header] = color_mid;
    style.Colors[ImGuiCol_HeaderHovered] = color_light;
    style.Colors[ImGuiCol_HeaderActive] = color_spec;
    style.Colors[ImGuiCol_Separator] = color_mid;
    style.Colors[ImGuiCol_SeparatorHovered] = color_light;
    style.Colors[ImGuiCol_SeparatorActive] = color_spec;
    style.Colors[ImGuiCol_ResizeGrip] = color_mid;
    style.Colors[ImGuiCol_ResizeGripHovered] = color_light;
    style.Colors[ImGuiCol_ResizeGripActive] = color_spec;
    style.Colors[ImGuiCol_Tab] = color_mid;
    style.Colors[ImGuiCol_TabHovered] = color_spec;
    style.Colors[ImGuiCol_TabActive] = color_spec;
    style.Colors[ImGuiCol_TabUnfocused] = color_mid;
    style.Colors[ImGuiCol_TabUnfocusedActive] = color_light;
    style.Colors[ImGuiCol_DockingPreview] = color_spec;
    style.Colors[ImGuiCol_DockingEmptyBg] = color_mid;
    style.Colors[ImGuiCol_PlotLines] = color_spec;
    style.Colors[ImGuiCol_PlotLinesHovered] = color_light;
    style.Colors[ImGuiCol_PlotHistogram] = color_spec;
    style.Colors[ImGuiCol_PlotHistogramHovered] = color_light;
    style.Colors[ImGuiCol_TableHeaderBg] = color_mid;
    style.Colors[ImGuiCol_TableBorderStrong] = color_dark;
    style.Colors[ImGuiCol_TableBorderLight] = color_light;
    style.Colors[ImGuiCol_TableRowBg] = color_dark;
    style.Colors[ImGuiCol_TableRowBgAlt] = color_mid;
    style.Colors[ImGuiCol_TextSelectedBg] = color_light;
    style.Colors[ImGuiCol_DragDropTarget] = color_spec;
    style.Colors[ImGuiCol_NavHighlight] = color_spec;
    style.Colors[ImGuiCol_NavWindowingHighlight] = color_spec;
    style.Colors[ImGuiCol_NavWindowingDimBg] = color_mid;
    style.Colors[ImGuiCol_ModalWindowDimBg] = color_mid;
}

void load_custom_font(const editor_state* state)
{
    const std::string font_path = state->gui_state->builtin_path + "fonts/JetBrainsMono.ttf";
    constexpr int font_size = 16;

    const ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF(font_path.c_str(), font_size);
}
