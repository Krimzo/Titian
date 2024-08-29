#include "titian.h"


titian::CodeEditor::CodeEditor()
	: m_text_editor()
{}

void titian::CodeEditor::load(const StringView& source)
{
	m_text_editor.SetText(source);
}

void titian::CodeEditor::edit(String* source)
{
	m_text_editor.Render("Interp Editor");
	*source = m_text_editor.GetText();
}

titian::String titian::CodeEditor::get_word_at_cursor() const
{
	return m_text_editor.get_word_at_cursor();
}

void titian::CodeEditor::replace_word_at_cursor(const StringView& text)
{
	m_text_editor.replace_word_at_cursor(text);
}

const TextEditor::LanguageDefinition* titian::CodeEditor::get_definition() const
{
	return m_text_editor.mLanguageDefinition.get();
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
