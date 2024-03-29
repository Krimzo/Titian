#pragma once

#include "klibrary.h"


namespace titian {
	class LanguageEditor
	{
	public:
		LanguageEditor();

		void load(const std::string& source);
		void edit(std::string* source);

		void load_chai_standard();
		void load_hlsl_standard();

	private:
		TextEditor m_text_editor;
	};
}
