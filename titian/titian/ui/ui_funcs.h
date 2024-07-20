#pragma once

#include "standard/standard.h"


namespace titian {
	// misc
	void ui_separator();
	void ui_same_line();
	void ui_set_next_width(float width);

	// cursor
	kl::Float2 ui_cursor_pos();
	void ui_set_cursor_pos(const kl::Float2& pos);

	// window
	void ui_window(const String& name, const Function<void()>& func);
	void ui_button(const String& name, const Function<void()>& func);
	void ui_checkbox(const String& name, bool& value);

	// text
	void ui_text(const String& data);
	void ui_colored_text(const String& data, const kl::Float4& color);

	// input
	void ui_input_int(const String& name, int& value);
	void ui_input_float(const String& name, float& value);

	void ui_input_float2(const String& name, kl::Float2& value);
	void ui_input_float3(const String& name, kl::Float3& value);
	void ui_input_float4(const String& name, kl::Float4& value);

	void ui_input_text(const String& name, String& value);
	void ui_input_text_multiline(const String& name, String& value);

	// edit
	void ui_edit_color3(const String& name, kl::Float3& value);
	void ui_edit_color4(const String& name, kl::Float4& value);

	// drag
	void ui_drag_int(const String& name, int& value);
	void ui_drag_float(const String& name, float& value);

	void ui_drag_float2(const String& name, float& value);
	void ui_drag_float3(const String& name, float& value);
	void ui_drag_float4(const String& name, float& value);

	// slide
	void ui_slide_int(const String& name, int& value, int min, int max);
	void ui_slide_float(const String& name, float& value, float min, float max);
}
