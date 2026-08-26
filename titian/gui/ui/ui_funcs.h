#pragma once

#include "globals.h"

namespace titian
{
void ui_separator();
void ui_same_line();
void ui_set_next_width(float width);

float2 ui_cursor_pos();
void ui_set_cursor_pos(float2 pos);

void ui_window(StringRef const& name, Func<void()> const& func);
void ui_button(StringRef const& name, Func<void()> const& func);

void ui_bool(StringRef const& name, bool value, Func<void(bool)> const& func);
void ui_int(StringRef const& name, int value, Func<void(int)> const& func);
void ui_int2(StringRef const& name, int2 value, Func<void(int2)> const& func);
void ui_float(StringRef const& name, float value, Func<void(float)> const& func);
void ui_float2(StringRef const& name, float2 value, Func<void(float2)> const& func);
void ui_float3(StringRef const& name, float3 value, Func<void(float3)> const& func);
void ui_float4(StringRef const& name, float4 value, Func<void(float4)> const& func);
void ui_color(StringRef const& name, RGB value, Func<void(RGB)> const& func);
void ui_string(StringRef const& name, String value, Func<void(String)> const& func);

void ui_text(StringRef const& data);
void ui_colored_text(float4 const& color, StringRef const& data);
} // namespace titian
