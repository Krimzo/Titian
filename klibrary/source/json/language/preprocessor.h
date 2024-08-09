#pragma once

#include "json/language/standard.h"


namespace kl::json {
	class Preprocessor : public std::unordered_map<std::string, std::string>
	{
	public:
		Preprocessor();

		void process(std::string& source) const;

	private:
		void apply_macros(std::string& source) const;
		void remove_whitespace(std::string& source) const;
	};
}
