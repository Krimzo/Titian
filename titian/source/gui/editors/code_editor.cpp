#include "titian.h"


titian::CodeEditor::CodeEditor()
	: m_text_editor()
{}

void titian::CodeEditor::load(const StringView& source)
{
	m_text_editor.SetText(source);
}

void titian::CodeEditor::edit(String& source)
{
	m_text_editor.Update("Interp Editor");
	source = m_text_editor.GetText();
}

void titian::CodeEditor::load_lua_standard()
{
	auto ptr = TextEditor::LanguageDefinition::lua(LUA_KEYWORDS, LUA_TYPES, LUA_MEMBERS, LUA_FUNCTIONS);
	m_text_editor.SetLanguageDefinition(ptr);
}

void titian::CodeEditor::load_hlsl_standard()
{
	auto ptr = TextEditor::LanguageDefinition::hlsl(HLSL_KEYWORDS, HLSL_TYPES, HLSL_MEMBERS, HLSL_FUNCTIONS);
	m_text_editor.SetLanguageDefinition(ptr);
}
