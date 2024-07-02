#include "main.h"


// misc
void titian::ui_separator()
{
	ImGui::Separator();
}

void titian::ui_same_line()
{
	ImGui::SameLine();
}

void titian::ui_set_next_width(const float width)
{
	ImGui::SetNextItemWidth(width);
}

// cursor
kl::Float2 titian::ui_cursor_pos()
{
	return { ImGui::GetCursorPosX(), ImGui::GetCursorPosY() };
}

void titian::ui_set_cursor_pos(const kl::Float2& pos)
{
	ImGui::SetCursorPos({ pos.x, pos.y });
}

// window
void titian::ui_window(const std::string& name, const std::function<void()>& func)
{
	if (ImGui::Begin(name.c_str(), nullptr, NULL)) {
		func();
	}
	ImGui::End();
}

void titian::ui_button(const std::string& name, const std::function<void()>& func)
{
	if (ImGui::Button(name.c_str())) {
		func();
	}
}

void titian::ui_checkbox(const std::string& name, bool& value)
{
	ImGui::Checkbox(name.c_str(), &value);
}

// text
void titian::ui_text(const std::string& data)
{
	ImGui::Text(data.c_str());
}

void titian::ui_colored_text(const std::string& data, const kl::Float4& color)
{
	ImGui::TextColored(ImVec4(color.x, color.y, color.z, color.w), data.c_str());
}

// input
void titian::ui_input_int(const std::string& name, int& value)
{
	ImGui::InputInt(name.c_str(), &value);
}

void titian::ui_input_float(const std::string& name, float& value)
{
	ImGui::InputFloat(name.c_str(), &value);
}

void titian::ui_input_float2(const std::string& name, kl::Float2& value)
{
	ImGui::InputFloat2(name.c_str(), value);
}

void titian::ui_input_float3(const std::string& name, kl::Float3& value)
{
	ImGui::InputFloat3(name.c_str(), value);
}

void titian::ui_input_float4(const std::string& name, kl::Float4& value)
{
	ImGui::InputFloat4(name.c_str(), value);
}

void titian::ui_input_text(const std::string& name, std::string& value)
{
	ImGui::InputText(name.c_str(), &value);
}

void titian::ui_input_text_multiline(const std::string& name, std::string& value)
{
	ImGui::InputTextMultiline(name.c_str(), &value);
}

// edit
void titian::ui_edit_color3(const std::string& name, kl::Float3& value)
{
	ImGui::ColorEdit3(name.c_str(), value);
}

void titian::ui_edit_color4(const std::string& name, kl::Float4& value)
{
	ImGui::ColorEdit4(name.c_str(), value);
}

// drag
void titian::ui_drag_int(const std::string& name, int& value)
{
	ImGui::DragInt(name.c_str(), &value);
}

void titian::ui_drag_float(const std::string& name, float& value)
{
	ImGui::DragFloat(name.c_str(), &value);
}

void titian::ui_drag_float2(const std::string& name, float& value)
{
	ImGui::DragFloat2(name.c_str(), &value);
}

void titian::ui_drag_float3(const std::string& name, float& value)
{
	ImGui::DragFloat3(name.c_str(), &value);
}

void titian::ui_drag_float4(const std::string& name, float& value)
{
	ImGui::DragFloat4(name.c_str(), &value);
}

// slide
void titian::ui_slide_int(const std::string& name, int& value, int min, int max)
{
	ImGui::SliderInt(name.c_str(), &value, min, max);
}

void titian::ui_slide_float(const std::string& name, float& value, float min, float max)
{
	ImGui::SliderFloat(name.c_str(), &value, min, max);
}
