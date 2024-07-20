#include "main.h"


// misc
void titian::ui_separator()
{
	im::Separator();
}

void titian::ui_same_line()
{
	im::SameLine();
}

void titian::ui_set_next_width(const float width)
{
	im::SetNextItemWidth(width);
}

// cursor
titian::Float2 titian::ui_cursor_pos()
{
	return { im::GetCursorPosX(), im::GetCursorPosY() };
}

void titian::ui_set_cursor_pos(const Float2& pos)
{
	im::SetCursorPos({ pos.x, pos.y });
}

// window
void titian::ui_window(const String& name, const Function<void()>& func)
{
	if (im::Begin(name.c_str(), nullptr, NULL)) {
		func();
	}
	im::End();
}

void titian::ui_button(const String& name, const Function<void()>& func)
{
	if (im::Button(name.c_str())) {
		func();
	}
}

void titian::ui_checkbox(const String& name, bool& value)
{
	im::Checkbox(name.c_str(), &value);
}

// text
void titian::ui_text(const String& data)
{
	im::Text(data.c_str());
}

void titian::ui_colored_text(const String& data, const Float4& color)
{
	im::TextColored(ImVec4(color.x, color.y, color.z, color.w), data.c_str());
}

// input
void titian::ui_input_int(const String& name, int& value)
{
	im::InputInt(name.c_str(), &value);
}

void titian::ui_input_float(const String& name, float& value)
{
	im::InputFloat(name.c_str(), &value);
}

void titian::ui_input_float2(const String& name, Float2& value)
{
	im::InputFloat2(name.c_str(), value);
}

void titian::ui_input_float3(const String& name, Float3& value)
{
	im::InputFloat3(name.c_str(), value);
}

void titian::ui_input_float4(const String& name, Float4& value)
{
	im::InputFloat4(name.c_str(), value);
}

void titian::ui_input_text(const String& name, String& value)
{
	im::InputText(name.c_str(), &value);
}

void titian::ui_input_text_multiline(const String& name, String& value)
{
	im::InputTextMultiline(name.c_str(), &value);
}

// edit
void titian::ui_edit_color3(const String& name, Float3& value)
{
	im::ColorEdit3(name.c_str(), value);
}

void titian::ui_edit_color4(const String& name, Float4& value)
{
	im::ColorEdit4(name.c_str(), value);
}

// drag
void titian::ui_drag_int(const String& name, int& value)
{
	im::DragInt(name.c_str(), &value);
}

void titian::ui_drag_float(const String& name, float& value)
{
	im::DragFloat(name.c_str(), &value);
}

void titian::ui_drag_float2(const String& name, float& value)
{
	im::DragFloat2(name.c_str(), &value);
}

void titian::ui_drag_float3(const String& name, float& value)
{
	im::DragFloat3(name.c_str(), &value);
}

void titian::ui_drag_float4(const String& name, float& value)
{
	im::DragFloat4(name.c_str(), &value);
}

// slide
void titian::ui_slide_int(const String& name, int& value, int min, int max)
{
	im::SliderInt(name.c_str(), &value, min, max);
}

void titian::ui_slide_float(const String& name, float& value, float min, float max)
{
	im::SliderFloat(name.c_str(), &value, min, max);
}
