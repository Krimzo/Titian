#pragma once

#include "standard.h"


namespace titian {
	// misc
	void ui_separator();
	void ui_same_line();
	void ui_set_next_width(float width);

	// cursor
	Float2 ui_cursor_pos();
	void ui_set_cursor_pos(const Float2& pos);

	// window
	void ui_window(const StringView& name, const Function<void()>& func);
	void ui_button(const StringView& name, const Function<void()>& func);
	void ui_checkbox(const StringView& name, bool& value);

	// text
	void ui_text(const StringView& data);
	void ui_colored_text(const StringView& data, const Float4& color);

	// input
	void ui_input_int(const StringView& name, int& value);
	void ui_input_int2(const StringView& name, Int2& value);
	void ui_input_int3(const StringView& name, Int3& value);
	void ui_input_int4(const StringView& name, Int4& value);

	void ui_input_float(const StringView& name, float& value);
	void ui_input_float2(const StringView& name, Float2& value);
	void ui_input_float3(const StringView& name, Float3& value);
	void ui_input_float4(const StringView& name, Float4& value);

	void ui_input_text(const StringView& name, String& value);
	void ui_input_text_multiline(const StringView& name, String& value);

	// edit
	void ui_edit_color3(const StringView& name, Color& value);
	void ui_edit_color4(const StringView& name, Color& value);

	// drag
	void ui_drag_int(const StringView& name, int& value);
	void ui_drag_int2(const StringView& name, Int2& value);
	void ui_drag_int3(const StringView& name, Int3& value);
	void ui_drag_int4(const StringView& name, Int4& value);

	void ui_drag_float(const StringView& name, float& value);
	void ui_drag_float2(const StringView& name, Float2& value);
	void ui_drag_float3(const StringView& name, Float3& value);
	void ui_drag_float4(const StringView& name, Float4& value);

	// slide
	void ui_slide_int(const StringView& name, int& value, int min, int max);
	void ui_slide_float(const StringView& name, float& value, float min, float max);
}
