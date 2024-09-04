#pragma once

#include "standard.h"


namespace titian {
	void ui_separator();
	void ui_same_line();
	void ui_set_next_width(float width);

	Float2 ui_cursor_pos();
	void ui_set_cursor_pos(const Float2& pos);

	void ui_window(const StringView& name, const Function<void()>& func);
	void ui_button(const StringView& name, const Function<void()>& func);

	void ui_bool(const StringView& name, bool value, const Function<void(bool)>& func);
	void ui_int(const StringView& name, int value, const Function<void(int)>& func);
	void ui_int2(const StringView& name, Int2 value, const Function<void(Int2)>& func);
	void ui_float(const StringView& name, float value, const Function<void(float)>& func);
	void ui_float2(const StringView& name, Float2 value, const Function<void(Float2)>& func);
	void ui_float3(const StringView& name, Float3 value, const Function<void(Float3)>& func);
	void ui_float4(const StringView& name, Float4 value, const Function<void(Float4)>& func);
	void ui_color(const StringView& name, Color value, const Function<void(Color)>& func);
	void ui_string(const StringView& name, String value, const Function<void(String)>& func);

	void ui_text(const StringView& data);
	void ui_colored_text(const Float4& color, const StringView& data);
}
