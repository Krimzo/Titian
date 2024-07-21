#pragma once

#include "standard.h"


namespace titian {
	using namespace standard;
}

namespace titian {
	class CodeEditor
	{
	public:
		CodeEditor();

		void load(const String& source);
		void edit(String* source);

		String get_word_at_cursor() const;
		void replace_word_at_cursor(const String& text);

		const TextEditor::LanguageDefinition* get_definition() const;
		const TextEditor::Palette* get_palette() const;

		void load_chai_standard();
		void load_hlsl_standard();

	private:
		TextEditor m_text_editor;
	};
}
