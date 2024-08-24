#include "titian.h"


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
void titian::ui_window(const StringView& name, const Function<void()>& func)
{
	if (im::Begin(name.data(), nullptr, NULL)) {
		func();
	}
	im::End();
}

void titian::ui_button(const StringView& name, const Function<void()>& func)
{
	if (im::Button(name.data())) {
		func();
	}
}

void titian::ui_checkbox(const StringView& name, bool& value)
{
	im::Checkbox(name.data(), &value);
}

// text
void titian::ui_text(const StringView& data)
{
	im::Text(data.data());
}

void titian::ui_colored_text(const StringView& data, const Float4& color)
{
	im::TextColored(ImVec4(color.x, color.y, color.z, color.w), data.data());
}

// input
void titian::ui_input_int(const StringView& name, int& value)
{
	im::InputInt(name.data(), &value);
}

void titian::ui_input_int2(const StringView& name, Int2& value)
{
	im::InputInt2(name.data(), &value.x);
}

void titian::ui_input_int3(const StringView& name, Int3& value)
{
	im::InputInt3(name.data(), &value.x);
}

void titian::ui_input_int4(const StringView& name, Int4& value)
{
	im::InputInt4(name.data(), &value.x);
}

void titian::ui_input_float(const StringView& name, float& value)
{
	im::InputFloat(name.data(), &value);
}

void titian::ui_input_float2(const StringView& name, Float2& value)
{
	im::InputFloat2(name.data(), &value.x);
}

void titian::ui_input_float3(const StringView& name, Float3& value)
{
	im::InputFloat3(name.data(), &value.x);
}

void titian::ui_input_float4(const StringView& name, Float4& value)
{
	im::InputFloat4(name.data(), &value.x);
}

void titian::ui_input_text(const StringView& name, String& value)
{
	im::InputText(name.data(), &value);
}

void titian::ui_input_text_multiline(const StringView& name, String& value)
{
	im::InputTextMultiline(name.data(), &value);
}

// edit
void titian::ui_edit_color3(const StringView& name, Color& value)
{
	Float3 temp_val = value;
	if (im::ColorEdit3(name.data(), &temp_val.x)) {
		value = temp_val;
	}
}

void titian::ui_edit_color4(const StringView& name, Color& value)
{
	Float4 temp_val = value;
	if (im::ColorEdit4(name.data(), &temp_val.x)) {
		value = temp_val;
	}
}

// drag
void titian::ui_drag_int(const StringView& name, int& value)
{
	im::DragInt(name.data(), &value);
}

void titian::ui_drag_int2(const StringView& name, Int2& value)
{
	im::DragInt2(name.data(), &value.x);
}

void titian::ui_drag_int3(const StringView& name, Int3& value)
{
	im::DragInt3(name.data(), &value.x);
}

void titian::ui_drag_int4(const StringView& name, Int4& value)
{
	im::DragInt4(name.data(), &value.x);
}

void titian::ui_drag_float(const StringView& name, float& value)
{
	im::DragFloat(name.data(), &value);
}

void titian::ui_drag_float2(const StringView& name, Float2& value)
{
	im::DragFloat2(name.data(), &value.x);
}

void titian::ui_drag_float3(const StringView& name, Float3& value)
{
	im::DragFloat3(name.data(), &value.x);
}

void titian::ui_drag_float4(const StringView& name, Float4& value)
{
	im::DragFloat4(name.data(), &value.x);
}

// slide
void titian::ui_slide_int(const StringView& name, int& value, int min, int max)
{
	im::SliderInt(name.data(), &value, min, max);
}

void titian::ui_slide_float(const StringView& name, float& value, float min, float max)
{
	im::SliderFloat(name.data(), &value, min, max);
}
