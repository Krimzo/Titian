#include "titian.h"


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

titian::Float2 titian::ui_cursor_pos()
{
	return { im::GetCursorPosX(), im::GetCursorPosY() };
}

void titian::ui_set_cursor_pos(const Float2 pos)
{
	im::SetCursorPos({ pos.x, pos.y });
}

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

void titian::ui_bool(const StringView& name, bool value, const Function<void(bool)>& func)
{
	if (im::Checkbox(name.data(), &value)) {
		func(value);
	}
}

void titian::ui_int(const StringView& name, int value, const Function<void(int)>& func)
{
	if (im::DragInt(name.data(), &value)) {
		func(value);
	}
}

void titian::ui_int2(const StringView& name, Int2 value, const Function<void(Int2)>& func)
{
	if (im::DragInt2(name.data(), &value.x)) {
		func(value);
	}
}

void titian::ui_float(const StringView& name, float value, const Function<void(float)>& func)
{
	if (im::DragFloat(name.data(), &value)) {
		func(value);
	}
}

void titian::ui_float2(const StringView& name, Float2 value, const Function<void(Float2)>& func)
{
	if (im::DragFloat2(name.data(), &value.x)) {
		func(value);
	}
}

void titian::ui_float3(const StringView& name, Float3 value, const Function<void(Float3)>& func)
{
	if (im::DragFloat3(name.data(), &value.x)) {
		func(value);
	}
}

void titian::ui_float4(const StringView& name, Float4 value, const Function<void(Float4)>& func)
{
	if (im::DragFloat4(name.data(), &value.x)) {
		func(value);
	}
}

void titian::ui_color(const StringView& name, RGB value, const Function<void(RGB)>& func)
{
	Float4 temp_val = value;
	if (im::ColorEdit4(name.data(), &temp_val.x)) {
		func(temp_val);
	}
}

void titian::ui_string(const StringView& name, String value, const Function<void(String)>& func)
{
	if (im::InputText(name.data(), &value)) {
		func(value);
	}
}

void titian::ui_text(const StringView& data)
{
	im::Text(data.data());
}

void titian::ui_colored_text(const Float4& color, const StringView& data)
{
	im::TextColored(ImVec4{ color.x, color.y, color.z, color.w }, data.data());
}
