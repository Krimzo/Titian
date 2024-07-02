#pragma once

#include "klibrary.h"


namespace titian {
	// misc
	void ui_separator();
	void ui_same_line();
	void ui_set_next_width(float width);

	// cursor
	kl::Float2 ui_cursor_pos();
	void ui_set_cursor_pos(const kl::Float2& pos);

	// window
	void ui_window(const std::string& name, const std::function<void()>& func);
	void ui_button(const std::string& name, const std::function<void()>& func);
	void ui_checkbox(const std::string& name, bool& value);

	// text
	void ui_text(const std::string& data);
	void ui_colored_text(const std::string& data, const kl::Float4& color);

	// input
	void ui_input_int(const std::string& name, int& value);
	void ui_input_float(const std::string& name, float& value);

	void ui_input_float2(const std::string& name, kl::Float2& value);
	void ui_input_float3(const std::string& name, kl::Float3& value);
	void ui_input_float4(const std::string& name, kl::Float4& value);

	void ui_input_text(const std::string& name, std::string& value);
	void ui_input_text_multiline(const std::string& name, std::string& value);

	// edit
	void ui_edit_color3(const std::string& name, kl::Float3& value);
	void ui_edit_color4(const std::string& name, kl::Float4& value);

	// drag
	void ui_drag_int(const std::string& name, int& value);
	void ui_drag_float(const std::string& name, float& value);

	void ui_drag_float2(const std::string& name, float& value);
	void ui_drag_float3(const std::string& name, float& value);
	void ui_drag_float4(const std::string& name, float& value);

	// slide
	void ui_slide_int(const std::string& name, int& value, int min, int max);
	void ui_slide_float(const std::string& name, float& value, float min, float max);
}
