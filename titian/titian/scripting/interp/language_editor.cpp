#include "main.h"


void titian::LanguageEditor::edit(std::string* source) const
{
	ImGui::InputTextMultiline("LanguageEditor", source, ImVec2(-1.0f, -1.0f), ImGuiInputTextFlags_AllowTabInput);
}

void titian::LanguageEditor::load_chai_standard()
{
}

void titian::LanguageEditor::load_hlsl_standard()
{
}
