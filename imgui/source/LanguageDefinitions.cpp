#include "imgui_text_editor.h"


static bool TokenizeCStyleString(const char* in_begin, const char* in_end, const char*& out_begin, const char*& out_end)
{
	const char* p = in_begin;

	if (*p == '"')
	{
		p++;

		while (p < in_end)
		{
			// handle end of string
			if (*p == '"')
			{
				out_begin = in_begin;
				out_end = p + 1;
				return true;
			}

			// handle escape character for "
			if (*p == '\\' && p + 1 < in_end && p[1] == '"')
				p++;

			p++;
		}
	}

	return false;
}

static bool TokenizeCStyleCharacterLiteral(const char* in_begin, const char* in_end, const char*& out_begin, const char*& out_end)
{
	const char* p = in_begin;

	if (*p == '\'')
	{
		p++;

		// handle escape characters
		if (p < in_end && *p == '\\')
			p++;

		if (p < in_end)
			p++;

		// handle end of character literal
		if (p < in_end && *p == '\'')
		{
			out_begin = in_begin;
			out_end = p + 1;
			return true;
		}
	}

	return false;
}

static bool TokenizeCStyleIdentifier(const char* in_begin, const char* in_end, const char*& out_begin, const char*& out_end)
{
	const char* p = in_begin;

	if ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') || *p == '_')
	{
		p++;

		while ((p < in_end) && ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') || (*p >= '0' && *p <= '9') || *p == '_'))
			p++;

		out_begin = in_begin;
		out_end = p;
		return true;
	}

	return false;
}

static bool TokenizeCStyleNumber(const char* in_begin, const char* in_end, const char*& out_begin, const char*& out_end)
{
	const char* p = in_begin;

	const bool startsWithNumber = *p >= '0' && *p <= '9';

	if (*p != '+' && *p != '-' && !startsWithNumber)
		return false;

	p++;

	bool hasNumber = startsWithNumber;

	while (p < in_end && (*p >= '0' && *p <= '9'))
	{
		hasNumber = true;

		p++;
	}

	if (hasNumber == false)
		return false;

	bool isFloat = false;
	bool isHex = false;
	bool isBinary = false;

	if (p < in_end)
	{
		if (*p == '.')
		{
			isFloat = true;

			p++;

			while (p < in_end && (*p >= '0' && *p <= '9'))
				p++;
		}
		else if (*p == 'x' || *p == 'X')
		{
			// hex formatted integer of the type 0xef80

			isHex = true;

			p++;

			while (p < in_end && ((*p >= '0' && *p <= '9') || (*p >= 'a' && *p <= 'f') || (*p >= 'A' && *p <= 'F')))
				p++;
		}
		else if (*p == 'b' || *p == 'B')
		{
			// binary formatted integer of the type 0b01011101

			isBinary = true;

			p++;

			while (p < in_end && (*p >= '0' && *p <= '1'))
				p++;
		}
	}

	if (isHex == false && isBinary == false)
	{
		// floating point exponent
		if (p < in_end && (*p == 'e' || *p == 'E'))
		{
			isFloat = true;

			p++;

			if (p < in_end && (*p == '+' || *p == '-'))
				p++;

			bool hasDigits = false;

			while (p < in_end && (*p >= '0' && *p <= '9'))
			{
				hasDigits = true;

				p++;
			}

			if (hasDigits == false)
				return false;
		}

		// single precision floating point type
		if (p < in_end && *p == 'f')
			p++;
	}

	if (isFloat == false)
	{
		// integer size type
		while (p < in_end && (*p == 'u' || *p == 'U' || *p == 'l' || *p == 'L'))
			p++;
	}

	out_begin = in_begin;
	out_end = p;
	return true;
}

static bool TokenizeCStylePunctuation(const char* in_begin, const char* in_end, const char*& out_begin, const char*& out_end)
{
	(void)in_end;

	switch (*in_begin)
	{
	case '[':
	case ']':
	case '{':
	case '}':
	case '!':
	case '%':
	case '^':
	case '&':
	case '*':
	case '(':
	case ')':
	case '-':
	case '+':
	case '=':
	case '~':
	case '|':
	case '<':
	case '>':
	case '?':
	case ':':
	case '/':
	case ';':
	case ',':
	case '.':
		out_begin = in_begin;
		out_end = in_begin + 1;
		return true;
	}

	return false;
}

static bool TokenizeLuaStyleString(const char* in_begin, const char* in_end, const char*& out_begin, const char*& out_end)
{
	const char* p = in_begin;

	bool is_single_quote = false;
	bool is_double_quotes = false;
	bool is_double_square_brackets = false;

	switch (*p)
	{
	case '\'':
		is_single_quote = true;
		break;
	case '"':
		is_double_quotes = true;
		break;
	case '[':
		p++;
		if (p < in_end && *(p) == '[')
			is_double_square_brackets = true;
		break;
	}

	if (is_single_quote || is_double_quotes || is_double_square_brackets)
	{
		p++;

		while (p < in_end)
		{
			// handle end of string
			if ((is_single_quote && *p == '\'') || (is_double_quotes && *p == '"') || (is_double_square_brackets && *p == ']' && p + 1 < in_end && *(p + 1) == ']'))
			{
				out_begin = in_begin;

				if (is_double_square_brackets)
					out_end = p + 2;
				else
					out_end = p + 1;

				return true;
			}

			// handle escape character for "
			if (*p == '\\' && p + 1 < in_end && (is_single_quote || is_double_quotes))
				p++;

			p++;
		}
	}

	return false;
}

static bool TokenizeLuaStyleIdentifier(const char* in_begin, const char* in_end, const char*& out_begin, const char*& out_end)
{
	const char* p = in_begin;

	if ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') || *p == '_')
	{
		p++;

		while ((p < in_end) && ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') || (*p >= '0' && *p <= '9') || *p == '_'))
			p++;

		out_begin = in_begin;
		out_end = p;
		return true;
	}

	return false;
}

static bool TokenizeLuaStyleNumber(const char* in_begin, const char* in_end, const char*& out_begin, const char*& out_end)
{
	const char* p = in_begin;

	const bool startsWithNumber = *p >= '0' && *p <= '9';

	if (*p != '+' && *p != '-' && !startsWithNumber)
		return false;

	p++;

	bool hasNumber = startsWithNumber;

	while (p < in_end && (*p >= '0' && *p <= '9'))
	{
		hasNumber = true;

		p++;
	}

	if (hasNumber == false)
		return false;

	if (p < in_end)
	{
		if (*p == '.')
		{
			p++;

			while (p < in_end && (*p >= '0' && *p <= '9'))
				p++;
		}

		// floating point exponent
		if (p < in_end && (*p == 'e' || *p == 'E'))
		{
			p++;

			if (p < in_end && (*p == '+' || *p == '-'))
				p++;

			bool hasDigits = false;

			while (p < in_end && (*p >= '0' && *p <= '9'))
			{
				hasDigits = true;

				p++;
			}

			if (hasDigits == false)
				return false;
		}
	}

	out_begin = in_begin;
	out_end = p;
	return true;
}

static bool TokenizeLuaStylePunctuation(const char* in_begin, const char* in_end, const char*& out_begin, const char*& out_end)
{
	(void)in_end;

	switch (*in_begin)
	{
	case '[':
	case ']':
	case '{':
	case '}':
	case '!':
	case '%':
	case '#':
	case '^':
	case '&':
	case '*':
	case '(':
	case ')':
	case '-':
	case '+':
	case '=':
	case '~':
	case '|':
	case '<':
	case '>':
	case '?':
	case ':':
	case '/':
	case ';':
	case ',':
	case '.':
		out_begin = in_begin;
		out_end = in_begin + 1;
		return true;
	}

	return false;
}

std::shared_ptr<TextEditor::LanguageDefinition> TextEditor::LanguageDefinition::chai(
	const std::set<std::string, std::less<>>& keywords,
	const std::set<std::string, std::less<>>& types,
	const std::set<std::string, std::less<>>& members,
	const std::set<std::string, std::less<>>& functions)
{
	auto ptr = std::make_shared<LanguageDefinition>();

	for (auto& name : keywords) {
		ptr->mKeywords.insert(name);
	}
	for (auto& name : types) {
		ptr->mTypes[name] = {};
	}
	for (auto& name : members) {
		ptr->mMembers[name] = {};
	}
	for (auto& name : functions) {
		ptr->mFunctions[name] = {};
	}

	ptr->mTokenRegexStrings.push_back(std::make_pair<std::string, PaletteIndex>(R"##(L?\"(\\.|[^\"])*\")##", PaletteIndex::String));
	ptr->mTokenRegexStrings.push_back(std::make_pair<std::string, PaletteIndex>(R"##(\'\\?[^\']\')##", PaletteIndex::String));
	ptr->mTokenRegexStrings.push_back(std::make_pair<std::string, PaletteIndex>(R"##([+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)([eE][+-]?[0-9]+)?[fF]?)##", PaletteIndex::Number));
	ptr->mTokenRegexStrings.push_back(std::make_pair<std::string, PaletteIndex>(R"##([+-]?[0-9]+[Uu]?[lL]?[lL]?)##", PaletteIndex::Number));
	ptr->mTokenRegexStrings.push_back(std::make_pair<std::string, PaletteIndex>(R"##(0[0-7]+[Uu]?[lL]?[lL]?)##", PaletteIndex::Number));
	ptr->mTokenRegexStrings.push_back(std::make_pair<std::string, PaletteIndex>(R"##(0[xX][0-9a-fA-F]+[uU]?[lL]?[lL]?)##", PaletteIndex::Number));
	ptr->mTokenRegexStrings.push_back(std::make_pair<std::string, PaletteIndex>(R"##([a-zA-Z_][a-zA-Z0-9_]*)##", PaletteIndex::Identifier));
	ptr->mTokenRegexStrings.push_back(std::make_pair<std::string, PaletteIndex>(R"##([\[\]\{\}\!\%\^\&\*\(\)\-\+\=\~\|\<\>\?\/\;\,\.])##", PaletteIndex::Punctuation));

	ptr->mCommentStart = "/*";
	ptr->mCommentEnd = "*/";
	ptr->mSingleLineComment = "//";

	ptr->mCaseSensitive = true;
	ptr->mName = "CHAI";

	return ptr;
}

std::shared_ptr<TextEditor::LanguageDefinition> TextEditor::LanguageDefinition::hlsl(
	const std::set<std::string, std::less<>>& keywords,
	const std::set<std::string, std::less<>>& types,
	const std::set<std::string, std::less<>>& members,
	const std::set<std::string, std::less<>>& functions)
{
	auto ptr = std::make_shared<LanguageDefinition>();

	for (auto& name : keywords) {
		ptr->mKeywords.insert(name);
	}
	for (auto& name : types) {
		ptr->mTypes[name] = {};
	}
	for (auto& name : members) {
		ptr->mMembers[name] = {};
	}
	for (auto& name : functions) {
		ptr->mFunctions[name] = {};
	}

	ptr->mTokenRegexStrings.push_back(std::make_pair<std::string, PaletteIndex>(R"##([ \t]*#[ \t]*[a-zA-Z_]+)##", PaletteIndex::Preprocessor));
	ptr->mTokenRegexStrings.push_back(std::make_pair<std::string, PaletteIndex>(R"##(L?\"(\\.|[^\"])*\")##", PaletteIndex::String));
	ptr->mTokenRegexStrings.push_back(std::make_pair<std::string, PaletteIndex>(R"##(\'\\?[^\']\')##", PaletteIndex::CharLiteral));
	ptr->mTokenRegexStrings.push_back(std::make_pair<std::string, PaletteIndex>(R"##([+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)([eE][+-]?[0-9]+)?[fF]?)##", PaletteIndex::Number));
	ptr->mTokenRegexStrings.push_back(std::make_pair<std::string, PaletteIndex>(R"##([+-]?[0-9]+[Uu]?[lL]?[lL]?)##", PaletteIndex::Number));
	ptr->mTokenRegexStrings.push_back(std::make_pair<std::string, PaletteIndex>(R"##(0[0-7]+[Uu]?[lL]?[lL]?)##", PaletteIndex::Number));
	ptr->mTokenRegexStrings.push_back(std::make_pair<std::string, PaletteIndex>(R"##(0[xX][0-9a-fA-F]+[uU]?[lL]?[lL]?)##", PaletteIndex::Number));
	ptr->mTokenRegexStrings.push_back(std::make_pair<std::string, PaletteIndex>(R"##([a-zA-Z_][a-zA-Z0-9_]*)##", PaletteIndex::Identifier));
	ptr->mTokenRegexStrings.push_back(std::make_pair<std::string, PaletteIndex>(R"##([\[\]\{\}\!\%\^\&\*\(\)\-\+\=\~\|\<\>\?\/\;\,\.])##", PaletteIndex::Punctuation));

	ptr->mCommentStart = "/*";
	ptr->mCommentEnd = "*/";
	ptr->mSingleLineComment = "//";

	ptr->mCaseSensitive = true;
	ptr->mName = "HLSL";

	return ptr;
}
