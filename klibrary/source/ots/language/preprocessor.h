#pragma once

#include "ots/language/standard.h"


namespace kl::ots {
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
