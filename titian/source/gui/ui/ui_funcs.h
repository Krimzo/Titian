#pragma once

#include "standard.h"


namespace titian {
	void ui_separator();
	void ui_same_line();
	void ui_set_next_width(float width);

	Float2 ui_cursor_pos();
	void ui_set_cursor_pos(Float2 pos);

	void ui_window(const StringRef& name, const Func<void()>& func);
	void ui_button(const StringRef& name, const Func<void()>& func);

	void ui_bool(const StringRef& name, bool value, const Func<void(bool)>& func);
	void ui_int(const StringRef& name, int value, const Func<void(int)>& func);
	void ui_int2(const StringRef& name, Int2 value, const Func<void(Int2)>& func);
	void ui_float(const StringRef& name, float value, const Func<void(float)>& func);
	void ui_float2(const StringRef& name, Float2 value, const Func<void(Float2)>& func);
	void ui_float3(const StringRef& name, Float3 value, const Func<void(Float3)>& func);
	void ui_float4(const StringRef& name, Float4 value, const Func<void(Float4)>& func);
	void ui_color(const StringRef& name, RGB value, const Func<void(RGB)>& func);
	void ui_string(const StringRef& name, String value, const Func<void(String)>& func);

	void ui_text(const StringRef& data);
	void ui_colored_text(const Float4& color, const StringRef& data);
}
