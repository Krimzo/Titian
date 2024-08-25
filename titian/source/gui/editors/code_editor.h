#pragma once

#include "standard.h"


namespace titian {
	class CodeEditor
	{
	public:
		CodeEditor();

		void load(const StringView& source);
		void edit(String* source);

		String get_word_at_cursor() const;
		void replace_word_at_cursor(const StringView& text);

		const TextEditor::LanguageDefinition* get_definition() const;

		void load_chai_standard();
		void load_hlsl_standard();

	private:
		TextEditor m_text_editor;
	};
}
