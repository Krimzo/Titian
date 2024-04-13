#pragma once

#include "klibrary.h"


namespace titian {
	class LanguageEditor
	{
	public:
		LanguageEditor();

		void load(const std::string& source);
		void edit(std::string* source);

		std::string get_word_at_cursor() const;

		const TextEditor::LanguageDefinition* get_definition() const;
		const TextEditor::Palette* get_palette() const;

		void load_chai_standard();
		void load_hlsl_standard();

	private:
		TextEditor m_text_editor;
	};
}
