#include "main.h"


titian::LanguageEditor::LanguageEditor()
	: m_text_editor()
{
	static bool first_time_init = false;
	if (!first_time_init) {
		first_time_init = true;

		TextEditor::LanguageDefinition* definition = TextEditor::LanguageDefinition::Chai();
		definition->mKeywords = INTERP_SCRIPT_KEYWORDS;
		definition->mIdentifiers.clear();
		for (const auto& [name, info] : INTERP_SCRIPT_IDENTIFIERS) {
			TextEditor::Identifier identifier = {};
			identifier.mDeclaration = info;
			definition->mIdentifiers[name] = identifier;
		}
	}

	m_text_editor.SetPalette(TextEditor::PaletteId::Dark);
	m_text_editor.SetLanguageDefinition(TextEditor::LanguageDefinitionId::Chai);
}

void titian::LanguageEditor::load(const std::string& source)
{
	m_text_editor.SetText(source);
}

void titian::LanguageEditor::edit(std::string* source)
{
	m_text_editor.Render("Interp Editor");
	*source = m_text_editor.GetText();
}

void titian::LanguageEditor::load_chai_standard()
{
	m_text_editor.SetLanguageDefinition(TextEditor::LanguageDefinitionId::Chai);
}

void titian::LanguageEditor::load_hlsl_standard()
{
	m_text_editor.SetLanguageDefinition(TextEditor::LanguageDefinitionId::Hlsl);
}
