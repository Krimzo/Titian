#pragma once

#ifdef KL_USING_IMGUI

#define IMGUI_DEFINE_MATH_OPERATORS

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_stdlib.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <ImGuizmo.h>
#include <imgui_memory_editor.h>
#include <imgui_text_editor.h>
#include <ImNodeFlow.h>

namespace imgui = ImGui;

#endif
