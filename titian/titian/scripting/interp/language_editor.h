#pragma once

#include "klibrary.h"


namespace titian {
	class LanguageEditor
	{
	public:
		std::unordered_set<std::string> keywords;
		std::unordered_set<std::string> identifiers;

		LanguageEditor() = default;

		void edit(std::string* source) const;

		void load_chai_standard();
		void load_hlsl_standard();
	};
}
