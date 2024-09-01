#pragma once

#include "standard.h"


namespace titian {
	class CodeEditor
	{
	public:
		CodeEditor();

		void load(const StringView& source);
		void edit(String* source);

		void load_lua_standard();
		void load_hlsl_standard();

	private:
		TextEditor m_text_editor;
	};
}
