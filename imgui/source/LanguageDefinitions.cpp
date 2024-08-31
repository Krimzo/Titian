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

std::shared_ptr<TextEditor::LanguageDefinition> TextEditor::LanguageDefinition::lua(
	const std::set<std::string, std::less<>>& keywords,
	const std::set<std::string, std::less<>>& types,
	const std::set<std::string, std::less<>>& members,
	const std::set<std::string, std::less<>>& functions)
{
	auto ptr = std::make_shared<LanguageDefinition>();
	ptr->mName = "LUA";

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

	ptr->mCommentStart = "--[[";
	ptr->mCommentEnd = "]]";
	ptr->mSingleLineComment = "--";

	ptr->mTokenize = [](const char* in_begin, const char* in_end, const char*& out_begin, const char*& out_end, PaletteIndex& paletteIndex)
	{
		while (in_begin < in_end && isascii(*in_begin) && isblank(*in_begin))
			in_begin++;

		if (in_begin == in_end) {
			out_begin = in_end;
			out_end = in_end;
			paletteIndex = PaletteIndex::Default;
		}
		else if (TokenizeLuaStyleString(in_begin, in_end, out_begin, out_end)) paletteIndex = PaletteIndex::String;
		else if (TokenizeLuaStyleIdentifier(in_begin, in_end, out_begin, out_end)) paletteIndex = PaletteIndex::Identifier;
		else if (TokenizeLuaStyleNumber(in_begin, in_end, out_begin, out_end)) paletteIndex = PaletteIndex::Number;
		else if (TokenizeLuaStylePunctuation(in_begin, in_end, out_begin, out_end)) paletteIndex = PaletteIndex::Punctuation;
	};

	return ptr;
}

std::shared_ptr<TextEditor::LanguageDefinition> TextEditor::LanguageDefinition::hlsl(
	const std::set<std::string, std::less<>>& keywords,
	const std::set<std::string, std::less<>>& types,
	const std::set<std::string, std::less<>>& members,
	const std::set<std::string, std::less<>>& functions)
{
	auto ptr = std::make_shared<LanguageDefinition>();
	ptr->mName = "HLSL";

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

	ptr->mCommentStart = "/*";
	ptr->mCommentEnd = "*/";
	ptr->mSingleLineComment = "//";

	ptr->mTokenize = [](const char* in_begin, const char* in_end, const char*& out_begin, const char*& out_end, PaletteIndex& paletteIndex)
	{
		while (in_begin < in_end && isascii(*in_begin) && isblank(*in_begin))
			in_begin++;

		if (in_begin == in_end) {
			out_begin = in_end;
			out_end = in_end;
			paletteIndex = PaletteIndex::Default;
		}
		else if (TokenizeCStyleString(in_begin, in_end, out_begin, out_end)) paletteIndex = PaletteIndex::String;
		else if (TokenizeCStyleIdentifier(in_begin, in_end, out_begin, out_end)) paletteIndex = PaletteIndex::Identifier;
		else if (TokenizeCStyleNumber(in_begin, in_end, out_begin, out_end)) paletteIndex = PaletteIndex::Number;
		else if (TokenizeCStylePunctuation(in_begin, in_end, out_begin, out_end)) paletteIndex = PaletteIndex::Punctuation;
	};

	return ptr;
}
