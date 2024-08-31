#include <algorithm>
#include <string>
#include <cmath>
#include <set>

#include "imgui_text_editor.h"

#define IMGUI_SCROLLBAR_WIDTH 14.0f
#define POS_TO_COORDS_COLUMN_OFFSET 0.33f
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"


TextEditor::TextEditor()
{
	mLines.push_back(Line());
}

void TextEditor::SetLanguageDefinition(std::shared_ptr<LanguageDefinition> aValue)
{
	mLanguageDefinition = aValue;
	Colorize();
}

void TextEditor::SelectAll()
{
	ClearSelections();
	MoveTop();
	MoveBottom(true);
}

void TextEditor::SelectLine(int aLine)
{
	ClearSelections();
	SetSelection({ aLine, 0 }, { aLine, GetLineMaxColumn(aLine) });
}

void TextEditor::SelectRegion(int aStartLine, int aStartChar, int aEndLine, int aEndChar)
{
	ClearSelections();
	SetSelection(aStartLine, aStartChar, aEndLine, aEndChar);
}

void TextEditor::ClearSelections()
{
	mCursor.mEnd = mCursor.GetSelectionEnd();
	mCursor.mStart = mCursor.mEnd;
}

void TextEditor::SetCursorPosition(int aLine, int aCharIndex)
{
	SetCursorPosition({ aLine, GetCharacterColumn(aLine, aCharIndex) }, true);
}

int TextEditor::GetFirstVisibleLine()
{
	return mFirstVisibleLine;
}

int TextEditor::GetLastVisibleLine()
{
	return mLastVisibleLine;
}

void TextEditor::SetViewAtLine(int aLine, SetViewAtLineMode aMode)
{
	mSetViewAtLine = aLine;
	mSetViewAtLineMode = aMode;
}

void TextEditor::Copy()
{
	if (mCursor.HasSelection()) {
		std::string clipboardText = GetClipboardText();
		ImGui::SetClipboardText(clipboardText.c_str());
	}
	else {
		if (!mLines.empty()) {
			std::string str;
			auto& line = mLines[GetActualCursorCoordinates().mLine];
			for (auto& g : line)
				str.push_back(g.mChar);
			ImGui::SetClipboardText(str.c_str());
		}
	}
}

void TextEditor::Cut()
{
	if (!mCursor.HasSelection())
		return;

	UndoRecord u;
	u.mBefore = mCursor;

	Copy();
	u.mOperations.push_back({ GetSelectedText(), mCursor.GetSelectionStart(), mCursor.GetSelectionEnd(), UndoOperationType::Delete });
	DeleteSelection();

	u.mAfter = mCursor;
	AddUndo(u);
}

void TextEditor::Paste()
{
	std::string clipText = ImGui::GetClipboardText();
	if (clipText.length() > 0) {
		UndoRecord u;
		u.mBefore = mCursor;

		if (mCursor.HasSelection()) {
			u.mOperations.push_back({ GetSelectedText(), mCursor.GetSelectionStart(), mCursor.GetSelectionEnd(), UndoOperationType::Delete });
			DeleteSelection();
		}

		Coordinates start = GetActualCursorCoordinates();
		InsertTextAtCursor(clipText);
		u.mOperations.push_back({ clipText, start, GetActualCursorCoordinates(), UndoOperationType::Add });

		u.mAfter = mCursor;
		AddUndo(u);
	}
}

void TextEditor::Undo(int aSteps)
{
	while (CanUndo() && aSteps-- > 0)
		mUndoBuffer[--mUndoIndex].Undo(this);
}

void TextEditor::Redo(int aSteps)
{
	while (CanRedo() && aSteps-- > 0)
		mUndoBuffer[mUndoIndex++].Redo(this);
}

void TextEditor::SetText(const std::string_view& aText)
{
	mLines.clear();
	mLines.emplace_back(Line());
	for (auto chr : aText) {
		if (chr == '\r')
			continue;

		if (chr == '\n') {
			mLines.emplace_back(Line());
		}
		else {
			mLines.back().emplace_back(Glyph(chr, PaletteIndex::Default));
		}
	}
	mUndoBuffer.clear();
	mUndoIndex = 0;
	Colorize();
}

std::string TextEditor::GetText() const
{
	auto lastLine = (int)mLines.size() - 1;
	auto lastLineLength = GetLineMaxColumn(lastLine);
	Coordinates startCoords = Coordinates();
	Coordinates endCoords = Coordinates(lastLine, lastLineLength);
	return startCoords < endCoords ? GetText(startCoords, endCoords) : "";
}

void TextEditor::SetTextLines(const std::vector<std::string>& aLines)
{
	mLines.clear();
	if (aLines.empty()) {
		mLines.emplace_back(Line());
	}
	else {
		mLines.resize(aLines.size());
		for (size_t i = 0; i < aLines.size(); ++i) {
			const std::string& aLine = aLines[i];

			mLines[i].reserve(aLine.size());
			for (size_t j = 0; j < aLine.size(); ++j)
				mLines[i].emplace_back(Glyph(aLine[j], PaletteIndex::Default));
		}
	}
	mUndoBuffer.clear();
	mUndoIndex = 0;
	Colorize();
}

std::vector<std::string> TextEditor::GetTextLines() const
{
	std::vector<std::string> result;

	result.reserve(mLines.size());

	for (auto& line : mLines)
	{
		std::string text;

		text.resize(line.size());

		for (size_t i = 0; i < line.size(); ++i)
			text[i] = line[i].mChar;

		result.emplace_back(std::move(text));
	}

	return result;
}

bool TextEditor::Render(const char* aTitle, bool aParentIsFocused, const ImVec2& aSize, bool aBorder)
{
	if (mCursorPositionChanged)
		OnCursorPositionChanged();
	mCursorPositionChanged = false;

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::ColorConvertU32ToFloat4(PALETTE[(int)PaletteIndex::Background]));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

	ImGui::BeginChild(aTitle, aSize, aBorder, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoNavInputs);

	bool isFocused = ImGui::IsWindowFocused();
	HandleKeyboardInputs(aParentIsFocused);
	HandleMouseInputs();
	ColorizeInternal();
	Render(aParentIsFocused);
	DisplayPopup(aParentIsFocused);

	ImGui::EndChild();

	ImGui::PopStyleVar();
	ImGui::PopStyleColor();

	return isFocused;
}

static int UTF8CharLength(char c)
{
	if ((c & 0xFE) == 0xFC)
		return 6;
	if ((c & 0xFC) == 0xF8)
		return 5;
	if ((c & 0xF8) == 0xF0)
		return 4;
	else if ((c & 0xF0) == 0xE0)
		return 3;
	else if ((c & 0xE0) == 0xC0)
		return 2;
	return 1;
}

static inline int ImTextCharToUtf8(char* buf, int buf_size, unsigned int c)
{
	if (c < 0x80)
	{
		buf[0] = (char)c;
		return 1;
	}
	if (c < 0x800)
	{
		if (buf_size < 2) return 0;
		buf[0] = (char)(0xc0 + (c >> 6));
		buf[1] = (char)(0x80 + (c & 0x3f));
		return 2;
	}
	if (c >= 0xdc00 && c < 0xe000)
	{
		return 0;
	}
	if (c >= 0xd800 && c < 0xdc00)
	{
		if (buf_size < 4) return 0;
		buf[0] = (char)(0xf0 + (c >> 18));
		buf[1] = (char)(0x80 + ((c >> 12) & 0x3f));
		buf[2] = (char)(0x80 + ((c >> 6) & 0x3f));
		buf[3] = (char)(0x80 + ((c) & 0x3f));
		return 4;
	}
	{
		if (buf_size < 3) return 0;
		buf[0] = (char)(0xe0 + (c >> 12));
		buf[1] = (char)(0x80 + ((c >> 6) & 0x3f));
		buf[2] = (char)(0x80 + ((c) & 0x3f));
		return 3;
	}
}

static inline bool CharIsWordChar(char ch)
{
	int sizeInBytes = UTF8CharLength(ch);
	return sizeInBytes > 1 ||
		ch >= 'a' && ch <= 'z' ||
		ch >= 'A' && ch <= 'Z' ||
		ch >= '0' && ch <= '9' ||
		ch == '_';
}

TextEditor::UndoRecord::UndoRecord(const std::vector<UndoOperation>& aOperations, const Cursor& aBefore, const Cursor& aAfter)
	: mOperations(aOperations), mBefore(aBefore), mAfter(aAfter)
{
	for (const UndoOperation& o : mOperations) {
		assert(o.mStart <= o.mEnd);
	}
}

void TextEditor::UndoRecord::Undo(TextEditor* aEditor)
{
	for (int i = (int) mOperations.size() - 1; i > -1; i--)
	{
		const UndoOperation& operation = mOperations[i];
		if (!operation.mText.empty())
		{
			switch (operation.mType)
			{
			case UndoOperationType::Delete:
			{
				auto start = operation.mStart;
				aEditor->InsertTextAt(start, operation.mText.c_str());
				aEditor->Colorize(operation.mStart.mLine - 1, operation.mEnd.mLine - operation.mStart.mLine + 2);
				break;
			}
			case UndoOperationType::Add:
			{
				aEditor->DeleteRange(operation.mStart, operation.mEnd);
				aEditor->Colorize(operation.mStart.mLine - 1, operation.mEnd.mLine - operation.mStart.mLine + 2);
				break;
			}
			}
		}
	}

	aEditor->mCursor = mBefore;
	aEditor->EnsureCursorVisible();
}

void TextEditor::UndoRecord::Redo(TextEditor* aEditor)
{
	for (int i = 0; i < mOperations.size(); i++)
	{
		const UndoOperation& operation = mOperations[i];
		if (!operation.mText.empty())
		{
			switch (operation.mType)
			{
			case UndoOperationType::Delete:
			{
				aEditor->DeleteRange(operation.mStart, operation.mEnd);
				aEditor->Colorize(operation.mStart.mLine - 1, operation.mEnd.mLine - operation.mStart.mLine + 1);
				break;
			}
			case UndoOperationType::Add:
			{
				auto start = operation.mStart;
				aEditor->InsertTextAt(start, operation.mText.c_str());
				aEditor->Colorize(operation.mStart.mLine - 1, operation.mEnd.mLine - operation.mStart.mLine + 1);
				break;
			}
			}
		}
	}

	aEditor->mCursor = mAfter;
	aEditor->EnsureCursorVisible();
}

// ---------- Text editor internal functions --------- //

std::string TextEditor::GetText(const Coordinates& aStart, const Coordinates& aEnd) const
{
	assert(aStart < aEnd);

	std::string result;
	auto lstart = aStart.mLine;
	auto lend = aEnd.mLine;
	auto istart = GetCharacterIndexR(aStart);
	auto iend = GetCharacterIndexR(aEnd);
	size_t s = 0;

	for (size_t i = lstart; i < lend; i++)
		s += mLines[i].size();

	result.reserve(s + s / 8);

	while (istart < iend || lstart < lend)
	{
		if (lstart >= (int)mLines.size())
			break;

		auto& line = mLines[lstart];
		if (istart < (int)line.size())
		{
			result += line[istart].mChar;
			istart++;
		}
		else
		{
			istart = 0;
			++lstart;
			result += '\n';
		}
	}

	return result;
}

std::string TextEditor::GetClipboardText() const
{
	std::string result;
	if (mCursor.GetSelectionStart() < mCursor.GetSelectionEnd()) {
		if (result.length() != 0)
			result += '\n';
		result += GetText(mCursor.GetSelectionStart(), mCursor.GetSelectionEnd());
	}
	return result;
}

std::string TextEditor::GetSelectedText() const
{
	return GetText(mCursor.GetSelectionStart(), mCursor.GetSelectionEnd());
}

void TextEditor::SetCursorPosition(const Coordinates& aPosition, bool aClearSelection)
{
	mCursorPositionChanged = true;
	if (aClearSelection) {
		mCursor.mStart = aPosition;
	}
	if (mCursor.mEnd != aPosition) {
		mCursor.mEnd = aPosition;
		EnsureCursorVisible();
	}
}

int TextEditor::InsertTextAt(Coordinates& aWhere, const char* aValue)
{
	int cindex = GetCharacterIndexR(aWhere);
	int totalLines = 0;
	while (*aValue != '\0')
	{
		assert(!mLines.empty());

		if (*aValue == '\r') {
			++aValue;
		}
		else if (*aValue == '\n') {
			if (cindex < (int)mLines[aWhere.mLine].size()) {
				auto& newLine = InsertLine(aWhere.mLine + 1);
				auto& line = mLines[aWhere.mLine];
				AddGlyphsToLine(aWhere.mLine + 1, 0, line.begin() + cindex, line.end());
				RemoveGlyphsFromLine(aWhere.mLine, cindex);
			}
			else {
				InsertLine(aWhere.mLine + 1);
			}
			++aWhere.mLine;
			aWhere.mColumn = 0;
			cindex = 0;
			++totalLines;
			++aValue;
		}
		else {
			auto& line = mLines[aWhere.mLine];
			auto d = UTF8CharLength(*aValue);
			while (d-- > 0 && *aValue != '\0')
				AddGlyphToLine(aWhere.mLine, cindex++, Glyph(*aValue++, PaletteIndex::Default));
			aWhere.mColumn = GetCharacterColumn(aWhere.mLine, cindex);
		}
	}
	return totalLines;
}

void TextEditor::InsertTextAtCursor(const std::string& aValue)
{
	InsertTextAtCursor(aValue.c_str());
}

void TextEditor::InsertTextAtCursor(const char* aValue)
{
	if (!aValue)
		return;

	Coordinates pos = GetActualCursorCoordinates();
	const Coordinates start = std::min(pos, mCursor.GetSelectionStart());
	const int totalLines = pos.mLine - start.mLine + InsertTextAt(pos, aValue);
	
	SetCursorPosition(pos);
	Colorize(start.mLine - 1, totalLines + 2);
}

bool TextEditor::Move(int& aLine, int& aCharIndex, bool aLeft, bool aLockLine) const
{
	// assumes given char index is not in the middle of utf8 sequence
	// char index can be line.length()

	// invalid line
	if (aLine >= mLines.size())
		return false;

	if (aLeft)
	{
		if (aCharIndex == 0)
		{
			if (aLockLine || aLine == 0)
				return false;
			aLine--;
			aCharIndex = (int) mLines[aLine].size();
		}
		else
		{
			aCharIndex--;
			while (aCharIndex > 0 && IsUTFSequence(mLines[aLine][aCharIndex].mChar))
				aCharIndex--;
		}
	}
	else // right
	{
		if (aCharIndex == mLines[aLine].size())
		{
			if (aLockLine || aLine == mLines.size() - 1)
				return false;
			aLine++;
			aCharIndex = 0;
		}
		else
		{
			int seqLength = UTF8CharLength(mLines[aLine][aCharIndex].mChar);
			aCharIndex = std::min(aCharIndex + seqLength, (int)mLines[aLine].size());
		}
	}
	return true;
}

void TextEditor::MoveCharIndexAndColumn(int aLine, int& aCharIndex, int& aColumn) const
{
	assert(aLine < mLines.size());
	assert(aCharIndex < mLines[aLine].size());
	char c = mLines[aLine][aCharIndex].mChar;
	aCharIndex += UTF8CharLength(c);
	if (c == '\t')
		aColumn = (aColumn / mTabSize) * mTabSize + mTabSize;
	else
		aColumn++;
}

void TextEditor::MoveCoords(Coordinates& aCoords, MoveDirection aDirection, bool aWordMode, int aLineCount) const
{
	int charIndex = GetCharacterIndexR(aCoords);
	int lineIndex = aCoords.mLine;
	switch (aDirection)
	{
	case MoveDirection::Right:
		if (charIndex >= mLines[lineIndex].size())
		{
			if (lineIndex < mLines.size() - 1)
			{
				aCoords.mLine = std::max(0, std::min((int)mLines.size() - 1, lineIndex + 1));
				aCoords.mColumn = 0;
			}
		}
		else
		{
			Move(lineIndex, charIndex);
			int oneStepRightColumn = GetCharacterColumn(lineIndex, charIndex);
			if (aWordMode)
			{
				aCoords = FindWordEnd(aCoords);
				aCoords.mColumn = std::max(aCoords.mColumn, oneStepRightColumn);
			}
			else
				aCoords.mColumn = oneStepRightColumn;
		}
		break;
	case MoveDirection::Left:
		if (charIndex == 0)
		{
			if (lineIndex > 0)
			{
				aCoords.mLine = lineIndex - 1;
				aCoords.mColumn = GetLineMaxColumn(aCoords.mLine);
			}
		}
		else
		{
			Move(lineIndex, charIndex, true);
			aCoords.mColumn = GetCharacterColumn(lineIndex, charIndex);
			if (aWordMode)
				aCoords = FindWordStart(aCoords);
		}
		break;
	case MoveDirection::Up:
		aCoords.mLine = std::max(0, lineIndex - aLineCount);
		break;
	case MoveDirection::Down:
		aCoords.mLine = std::max(0, std::min((int)mLines.size() - 1, lineIndex + aLineCount));
		break;
	}
}

void TextEditor::MoveUp(int aAmount, bool aSelect)
{
	Coordinates newCoords = mCursor.mEnd;
	MoveCoords(newCoords, MoveDirection::Up, false, aAmount);
	SetCursorPosition(newCoords, !aSelect);
	EnsureCursorVisible();
}

void TextEditor::MoveDown(int aAmount, bool aSelect)
{
	assert(mCursor.mEnd.mColumn >= 0);
	Coordinates newCoords = mCursor.mEnd;
	MoveCoords(newCoords, MoveDirection::Down, false, aAmount);
	SetCursorPosition(newCoords, !aSelect);
	EnsureCursorVisible();
}

void TextEditor::MoveLeft(bool aSelect, bool aWordMode)
{
	if (mLines.empty())
		return;

	if (mCursor.HasSelection() && !aSelect && !aWordMode) {
		SetCursorPosition(mCursor.GetSelectionStart());
	}
	else {
		Coordinates newCoords = mCursor.mEnd;
		MoveCoords(newCoords, MoveDirection::Left, aWordMode);
		SetCursorPosition(newCoords, !aSelect);
	}
	EnsureCursorVisible();
}

void TextEditor::MoveRight(bool aSelect, bool aWordMode)
{
	if (mLines.empty())
		return;

	if (mCursor.HasSelection() && !aSelect && !aWordMode) {
		SetCursorPosition(mCursor.GetSelectionEnd());
	}
	else {
		Coordinates newCoords = mCursor.mEnd;
		MoveCoords(newCoords, MoveDirection::Right, aWordMode);
		SetCursorPosition(newCoords, !aSelect);
	}
	EnsureCursorVisible();
}

void TextEditor::MoveTop(bool aSelect)
{
	SetCursorPosition(Coordinates(0, 0), !aSelect);
}

void TextEditor::TextEditor::MoveBottom(bool aSelect)
{
	int maxLine = (int)mLines.size() - 1;
	Coordinates newPos = Coordinates(maxLine, GetLineMaxColumn(maxLine));
	SetCursorPosition(newPos, !aSelect);
}

void TextEditor::MoveHome(bool aSelect)
{
	SetCursorPosition(Coordinates(mCursor.mEnd.mLine, 0), !aSelect);
}

void TextEditor::MoveEnd(bool aSelect)
{
	int lindex = mCursor.mEnd.mLine;
	SetCursorPosition(Coordinates(lindex, GetLineMaxColumn(lindex)), !aSelect);
}

void TextEditor::EnterCharacter(ImWchar aChar, bool aShift)
{
	if (mCursor.HasSelection() && mCursor.GetSelectionStart().mLine != mCursor.GetSelectionEnd().mLine && aChar == '\t') {
		ChangeCurrentLinesIndentation(!aShift);
		return;
	}

	UndoRecord u;
	u.mBefore = mCursor;
	if (mCursor.HasSelection()) {
		u.mOperations.push_back({ GetSelectedText(), mCursor.GetSelectionStart(), mCursor.GetSelectionEnd(), UndoOperationType::Delete });
		DeleteSelection();
	}

	std::vector<Coordinates> coords;
	auto coord = GetActualCursorCoordinates();
	coords.push_back(coord);
	UndoOperation added;
	added.mType = UndoOperationType::Add;
	added.mStart = coord;

	assert(!mLines.empty());
	if (aChar == '\n') {
		InsertLine(coord.mLine + 1);
		auto& line = mLines[coord.mLine];
		auto& newLine = mLines[coord.mLine + 1];

		added.mText = "";
		added.mText += (char) aChar;
		for (int i = 0; i < line.size() && isascii(line[i].mChar) && isblank(line[i].mChar); ++i) {
			newLine.push_back(line[i]);
			added.mText += line[i].mChar;
		}

		const size_t whitespaceSize = newLine.size();
		auto cindex = GetCharacterIndexR(coord);
		AddGlyphsToLine(coord.mLine + 1, (int) newLine.size(), line.begin() + cindex, line.end());
		RemoveGlyphsFromLine(coord.mLine, cindex);
		SetCursorPosition(Coordinates(coord.mLine + 1, GetCharacterColumn(coord.mLine + 1, (int) whitespaceSize)));
	}
	else {
		char buf[7];
		int e = ImTextCharToUtf8(buf, 7, aChar);
		if (e > 0) {
			buf[e] = '\0';
			auto& line = mLines[coord.mLine];
			auto cindex = GetCharacterIndexR(coord);

			for (auto p = buf; *p != '\0'; p++, ++cindex) {
				AddGlyphToLine(coord.mLine, cindex, Glyph(*p, PaletteIndex::Default));
			}
			added.mText = buf;
			SetCursorPosition(Coordinates(coord.mLine, GetCharacterColumn(coord.mLine, cindex)));
		}
	}

	added.mEnd = GetActualCursorCoordinates();
	u.mOperations.push_back(added);

	u.mAfter = mCursor;
	AddUndo(u);

	for (const auto& coord : coords) {
		Colorize(coord.mLine - 1, 3);
	}
	EnsureCursorVisible();
}

void TextEditor::Backspace(bool aWordMode)
{
	if (mLines.empty()) {
		return;
	}
	if (mCursor.HasSelection()) {
		Delete(aWordMode);
		return;
	}
	if (mCursor.mEnd.mLine <= 0
		&& mCursor.mEnd.mColumn <= 0) {
		return;
	}
	Cursor stateBeforeDeleting = mCursor;
	MoveLeft(true, aWordMode);	
	OnCursorPositionChanged();
	Delete(aWordMode, &stateBeforeDeleting);
}

void TextEditor::Delete(bool aWordMode, const Cursor* aEditorState)
{
	if (mLines.empty()) {
		return;
	}
	if (mCursor.HasSelection()) {
		UndoRecord u;
		u.mBefore = aEditorState ? *aEditorState : mCursor;
		u.mOperations.push_back({ GetSelectedText(), mCursor.GetSelectionStart(), mCursor.GetSelectionEnd(), UndoOperationType::Delete });
		DeleteSelection();
		u.mAfter = mCursor;
		AddUndo(u);
		return;
	}
	if (mCursor.mEnd.mLine >= (mLines.size() - 1)
		&& mCursor.mEnd.mColumn >= mLines.back().size()) {
		return;
	}
	Cursor stateBeforeDeleting = mCursor;
	MoveRight(true, aWordMode);
	OnCursorPositionChanged();
	Delete(aWordMode, &stateBeforeDeleting);
}

void TextEditor::SetSelection(Coordinates aStart, Coordinates aEnd)
{
	Coordinates minCoords = Coordinates(0, 0);
	int maxLine = (int)mLines.size() - 1;
	Coordinates maxCoords = Coordinates(maxLine, GetLineMaxColumn(maxLine));
	if (aStart < minCoords)
		aStart = minCoords;
	else if (aStart > maxCoords)
		aStart = maxCoords;
	if (aEnd < minCoords)
		aEnd = minCoords;
	else if (aEnd > maxCoords)
		aEnd = maxCoords;

	mCursor.mStart = aStart;
	SetCursorPosition(aEnd, false);
}

void TextEditor::SetSelection(int aStartLine, int aStartChar, int aEndLine, int aEndChar)
{
	Coordinates startCoords = { aStartLine, GetCharacterColumn(aStartLine, aStartChar) };
	Coordinates endCoords = { aEndLine, GetCharacterColumn(aEndLine, aEndChar) };
	SetSelection(startCoords, endCoords);
}

bool TextEditor::FindMatchingBracket(int aLine, int aCharIndex, Coordinates& out)
{
	if (aLine > mLines.size() - 1)
		return false;
	int maxCharIndex = (int) mLines[aLine].size() - 1;
	if (aCharIndex > maxCharIndex)
		return false;

	int currentLine = aLine;
	int currentCharIndex = aCharIndex;
	int counter = 1;
	if (CLOSE_TO_OPEN_CHAR.find(mLines[aLine][aCharIndex].mChar) != CLOSE_TO_OPEN_CHAR.end())
	{
		char closeChar = mLines[aLine][aCharIndex].mChar;
		char openChar = CLOSE_TO_OPEN_CHAR.at(closeChar);
		while (Move(currentLine, currentCharIndex, true))
		{
			if (currentCharIndex < mLines[currentLine].size())
			{
				char currentChar = mLines[currentLine][currentCharIndex].mChar;
				if (currentChar == openChar)
				{
					counter--;
					if (counter == 0)
					{
						out = { currentLine, GetCharacterColumn(currentLine, currentCharIndex) };
						return true;
					}
				}
				else if (currentChar == closeChar)
					counter++;
			}
		}
	}
	else if (OPEN_TO_CLOSE_CHAR.find(mLines[aLine][aCharIndex].mChar) != OPEN_TO_CLOSE_CHAR.end())
	{
		char openChar = mLines[aLine][aCharIndex].mChar;
		char closeChar = OPEN_TO_CLOSE_CHAR.at(openChar);
		while (Move(currentLine, currentCharIndex))
		{
			if (currentCharIndex < mLines[currentLine].size())
			{
				char currentChar = mLines[currentLine][currentCharIndex].mChar;
				if (currentChar == closeChar)
				{
					counter--;
					if (counter == 0)
					{
						out = { currentLine, GetCharacterColumn(currentLine, currentCharIndex) };
						return true;
					}
				}
				else if (currentChar == openChar)
					counter++;
			}
		}
	}
	return false;
}

void TextEditor::ChangeCurrentLinesIndentation(bool aIncrease)
{
	UndoRecord u;
	u.mBefore = mCursor;

	for (int currentLine = mCursor.GetSelectionEnd().mLine; currentLine >= mCursor.GetSelectionStart().mLine; currentLine--) {
		if (Coordinates{ currentLine, 0 } == mCursor.GetSelectionEnd() && mCursor.GetSelectionEnd() != mCursor.GetSelectionStart()) // when selection ends at line start
			continue;

		if (aIncrease) {
			if (mLines[currentLine].size() > 0) {
				Coordinates lineStart = { currentLine, 0 };
				Coordinates insertionEnd = lineStart;
				InsertTextAt(insertionEnd, "\t"); // sets insertion end
				u.mOperations.push_back({ "\t", lineStart, insertionEnd, UndoOperationType::Add });
				Colorize(lineStart.mLine, 1);
			}
		}
		else {
			Coordinates start = { currentLine, 0 };
			Coordinates end = { currentLine, mTabSize };
			int charIndex = GetCharacterIndexL(end) - 1;
			while (charIndex > -1 && (mLines[currentLine][charIndex].mChar == ' ' || mLines[currentLine][charIndex].mChar == '\t')) charIndex--;
			bool onlySpaceCharactersFound = charIndex == -1;
			if (onlySpaceCharactersFound) {
				u.mOperations.push_back({ GetText(start, end), start, end, UndoOperationType::Delete });
				DeleteRange(start, end);
				Colorize(currentLine, 1);
			}
		}
	}

	if (u.mOperations.size() > 0)
		AddUndo(u);
}

void TextEditor::MoveUpCurrentLines()
{
	UndoRecord u;
	u.mBefore = mCursor;

	std::set<int> affectedLines;
	int minLine = -1;
	int maxLine = -1;
	for (int currentLine = mCursor.GetSelectionEnd().mLine; currentLine >= mCursor.GetSelectionStart().mLine; currentLine--) {
		if (Coordinates{ currentLine, 0 } == mCursor.GetSelectionEnd() && mCursor.GetSelectionEnd() != mCursor.GetSelectionStart()) // when selection ends at line start
			continue;
		affectedLines.insert(currentLine);
		minLine = minLine == -1 ? currentLine : (currentLine < minLine ? currentLine : minLine);
		maxLine = maxLine == -1 ? currentLine : (currentLine > maxLine ? currentLine : maxLine);
	}
	if (minLine == 0) // can't move up anymore
		return;

	Coordinates start = { minLine - 1, 0 };
	Coordinates end = { maxLine, GetLineMaxColumn(maxLine) };
	u.mOperations.push_back({ GetText(start, end), start, end, UndoOperationType::Delete });

	for (int line : affectedLines) // lines should be sorted here
		std::swap(mLines[line - 1], mLines[line]);

	mCursor.mStart.mLine -= 1;
	mCursor.mEnd.mLine -= 1;

	end = { maxLine, GetLineMaxColumn(maxLine) }; // this line is swapped with line above, need to find new max column
	u.mOperations.push_back({ GetText(start, end), start, end, UndoOperationType::Add });
	u.mAfter = mCursor;
	AddUndo(u);
}

void TextEditor::MoveDownCurrentLines()
{
	UndoRecord u;
	u.mBefore = mCursor;

	std::set<int> affectedLines;
	int minLine = -1;
	int maxLine = -1;

	for (int currentLine = mCursor.GetSelectionEnd().mLine; currentLine >= mCursor.GetSelectionStart().mLine; currentLine--) {
		if (Coordinates{ currentLine, 0 } == mCursor.GetSelectionEnd() && mCursor.GetSelectionEnd() != mCursor.GetSelectionStart()) // when selection ends at line start
			continue;
		affectedLines.insert(currentLine);
		minLine = minLine == -1 ? currentLine : (currentLine < minLine ? currentLine : minLine);
		maxLine = maxLine == -1 ? currentLine : (currentLine > maxLine ? currentLine : maxLine);
	}

	if (maxLine == mLines.size() - 1) // can't move down anymore
		return;

	Coordinates start = { minLine, 0 };
	Coordinates end = { maxLine + 1, GetLineMaxColumn(maxLine + 1)};
	u.mOperations.push_back({ GetText(start, end), start, end, UndoOperationType::Delete });

	std::set<int>::reverse_iterator rit;
	for (rit = affectedLines.rbegin(); rit != affectedLines.rend(); rit++) // lines should be sorted here
		std::swap(mLines[*rit + 1], mLines[*rit]);
	
	mCursor.mStart.mLine += 1;
	mCursor.mEnd.mLine += 1;

	end = { maxLine + 1, GetLineMaxColumn(maxLine + 1) }; // this line is swapped with line below, need to find new max column
	u.mOperations.push_back({ GetText(start, end), start, end, UndoOperationType::Add });
	u.mAfter = mCursor;
	AddUndo(u);
}

void TextEditor::ToggleLineComment()
{
	if (!mLanguageDefinition)
		return;
	const std::string& commentString = mLanguageDefinition->mSingleLineComment;

	UndoRecord u;
	u.mBefore = mCursor;

	bool shouldAddComment = false;
	std::unordered_set<int> affectedLines;
	for (int currentLine = mCursor.GetSelectionEnd().mLine; currentLine >= mCursor.GetSelectionStart().mLine; currentLine--) {
		if (Coordinates{ currentLine, 0 } == mCursor.GetSelectionEnd() && mCursor.GetSelectionEnd() != mCursor.GetSelectionStart()) // when selection ends at line start
			continue;
		affectedLines.insert(currentLine);
		int currentIndex = 0;
		while (currentIndex < mLines[currentLine].size() && (mLines[currentLine][currentIndex].mChar == ' ' || mLines[currentLine][currentIndex].mChar == '\t')) currentIndex++;
		if (currentIndex == mLines[currentLine].size())
			continue;
		int i = 0;
		while (i < commentString.length() && currentIndex + i < mLines[currentLine].size() && mLines[currentLine][currentIndex + i].mChar == commentString[i]) i++;
		bool matched = i == commentString.length();
		shouldAddComment |= !matched;
	}

	if (shouldAddComment)
	{
		for (int currentLine : affectedLines) // order doesn't matter as changes are not multiline
		{
			Coordinates lineStart = { currentLine, 0 };
			Coordinates insertionEnd = lineStart;
			InsertTextAt(insertionEnd, (commentString + ' ').c_str()); // sets insertion end
			u.mOperations.push_back({ (commentString + ' ') , lineStart, insertionEnd, UndoOperationType::Add });
			Colorize(lineStart.mLine, 1);
		}
	}
	else
	{
		for (int currentLine : affectedLines) // order doesn't matter as changes are not multiline
		{
			int currentIndex = 0;
			while (currentIndex < mLines[currentLine].size() && (mLines[currentLine][currentIndex].mChar == ' ' || mLines[currentLine][currentIndex].mChar == '\t')) currentIndex++;
			if (currentIndex == mLines[currentLine].size())
				continue;
			int i = 0;
			while (i < commentString.length() && currentIndex + i < mLines[currentLine].size() && mLines[currentLine][currentIndex + i].mChar == commentString[i]) i++;
			bool matched = i == commentString.length();
			assert(matched);
			if (currentIndex + i < mLines[currentLine].size() && mLines[currentLine][currentIndex + i].mChar == ' ')
				i++;

			Coordinates start = { currentLine, GetCharacterColumn(currentLine, currentIndex) };
			Coordinates end = { currentLine, GetCharacterColumn(currentLine, currentIndex + i) };
			u.mOperations.push_back({ GetText(start, end) , start, end, UndoOperationType::Delete});
			DeleteRange(start, end);
			Colorize(currentLine, 1);
		}
	}

	u.mAfter = mCursor;
	AddUndo(u);
}

void TextEditor::RemoveCurrentLines()
{
	UndoRecord u;
	u.mBefore = mCursor;

	if (mCursor.HasSelection()) {
		u.mOperations.push_back({ GetSelectedText(), mCursor.GetSelectionStart(), mCursor.GetSelectionEnd(), UndoOperationType::Delete });
		DeleteSelection();
	}
	MoveHome();
	OnCursorPositionChanged(); // might combine cursors

	int currentLine = mCursor.mEnd.mLine;
	int nextLine = currentLine + 1;
	int prevLine = currentLine - 1;

	Coordinates toDeleteStart, toDeleteEnd;
	if (mLines.size() > nextLine) {
		toDeleteStart = Coordinates(currentLine, 0);
		toDeleteEnd = Coordinates(nextLine, 0);
		SetCursorPosition({ mCursor.mEnd.mLine, 0 });
	}
	else if (prevLine > -1) {
		toDeleteStart = Coordinates(prevLine, GetLineMaxColumn(prevLine));
		toDeleteEnd = Coordinates(currentLine, GetLineMaxColumn(currentLine));
		SetCursorPosition({ prevLine, 0 });
	}
	else {
		toDeleteStart = Coordinates(currentLine, 0);
		toDeleteEnd = Coordinates(currentLine, GetLineMaxColumn(currentLine));
		SetCursorPosition({ currentLine, 0 });
	}

	u.mOperations.push_back({ GetText(toDeleteStart, toDeleteEnd), toDeleteStart, toDeleteEnd, UndoOperationType::Delete });

	if (toDeleteStart.mLine != toDeleteEnd.mLine) {
		RemoveLine(currentLine);
	}
	else {
		DeleteRange(toDeleteStart, toDeleteEnd);
	}

	u.mAfter = mCursor;
	AddUndo(u);
}

float TextEditor::TextDistanceToLineStart(const Coordinates& aFrom, bool aSanitizeCoords) const
{
	if (aSanitizeCoords) {
		return SanitizeCoordinates(aFrom).mColumn * mCharAdvance.x;
	}
	return aFrom.mColumn * mCharAdvance.x;
}

void TextEditor::EnsureCursorVisible(bool aStartToo)
{
	mEnsureCursorVisible = 0;
	mEnsureCursorVisibleStartToo = aStartToo;
}

TextEditor::Coordinates TextEditor::SanitizeCoordinates(const Coordinates& aValue) const
{
	auto line = aValue.mLine;
	auto column = aValue.mColumn;
	if (line >= (int) mLines.size()) {
		if (mLines.empty()) {
			line = 0;
			column = 0;
		}
		else {
			line = (int) mLines.size() - 1;
			column = GetLineMaxColumn(line);
		}
		return Coordinates(line, column);
	}
	column = mLines.empty() ? 0 : GetLineMaxColumn(line, column);
	return Coordinates(line, column);
}

TextEditor::Coordinates TextEditor::GetActualCursorCoordinates(bool aStart) const
{
	return SanitizeCoordinates(aStart ? mCursor.mStart : mCursor.mEnd);
}

TextEditor::Coordinates TextEditor::ScreenPosToCoordinates(const ImVec2& aPosition, bool* isOverLineNumber) const
{
	ImVec2 origin = ImGui::GetCursorScreenPos();
	ImVec2 local(aPosition.x - origin.x + 3.0f, aPosition.y - origin.y);

	if (isOverLineNumber != nullptr)
		*isOverLineNumber = local.x < mTextStart;

	Coordinates out = {
		std::max(0, (int)floor(local.y / mCharAdvance.y)),
		std::max(0, (int)floor((local.x - mTextStart) / mCharAdvance.x))
	};
	int charIndex = GetCharacterIndexL(out);
	if (charIndex > -1 && charIndex < mLines[out.mLine].size() && mLines[out.mLine][charIndex].mChar == '\t')
	{
		int columnToLeft = GetCharacterColumn(out.mLine, charIndex);
		int columnToRight = GetCharacterColumn(out.mLine, GetCharacterIndexR(out));
		if (out.mColumn - columnToLeft < columnToRight - out.mColumn)
			out.mColumn = columnToLeft;
		else
			out.mColumn = columnToRight;
	}
	else
		out.mColumn = std::max(0, (int)floor((local.x - mTextStart + POS_TO_COORDS_COLUMN_OFFSET * mCharAdvance.x) / mCharAdvance.x));
	return SanitizeCoordinates(out);
}

TextEditor::Coordinates TextEditor::FindWordStart(const Coordinates& aFrom) const
{
	if (aFrom.mLine >= (int)mLines.size())
		return aFrom;

	int lineIndex = aFrom.mLine;
	auto& line = mLines[lineIndex];
	int charIndex = GetCharacterIndexL(aFrom);

	if (charIndex > (int)line.size() || line.size() == 0)
		return aFrom;
	if (charIndex == (int)line.size())
		charIndex--;

	bool initialIsWordChar = CharIsWordChar(line[charIndex].mChar);
	bool initialIsSpace = isspace(line[charIndex].mChar);
	char initialChar = line[charIndex].mChar;
	while (Move(lineIndex, charIndex, true, true))
	{
		bool isWordChar = CharIsWordChar(line[charIndex].mChar);
		bool isSpace = isspace(line[charIndex].mChar);
		if (initialIsSpace && !isSpace ||
			initialIsWordChar && !isWordChar ||
			!initialIsWordChar && !initialIsSpace && initialChar != line[charIndex].mChar)
		{
			Move(lineIndex, charIndex, false, true); // one step to the right
			break;
		}
	}
	return { aFrom.mLine, GetCharacterColumn(aFrom.mLine, charIndex) };
}

TextEditor::Coordinates TextEditor::FindWordEnd(const Coordinates& aFrom) const
{
	if (aFrom.mLine >= (int)mLines.size())
		return aFrom;

	int lineIndex = aFrom.mLine;
	auto& line = mLines[lineIndex];
	auto charIndex = GetCharacterIndexL(aFrom);

	if (charIndex >= (int)line.size())
		return aFrom;

	bool initialIsWordChar = CharIsWordChar(line[charIndex].mChar);
	bool initialIsSpace = isspace(line[charIndex].mChar);
	char initialChar = line[charIndex].mChar;
	while (Move(lineIndex, charIndex, false, true))
	{
		if (charIndex == line.size())
			break;
		bool isWordChar = CharIsWordChar(line[charIndex].mChar);
		bool isSpace = isspace(line[charIndex].mChar);
		if (initialIsSpace && !isSpace ||
			initialIsWordChar && !isWordChar ||
			!initialIsWordChar && !initialIsSpace && initialChar != line[charIndex].mChar)
			break;
	}
	return { lineIndex, GetCharacterColumn(aFrom.mLine, charIndex) };
}

int TextEditor::GetCharacterIndexL(const Coordinates& aCoords) const
{
	if (aCoords.mLine >= mLines.size())
		return -1;

	auto& line = mLines[aCoords.mLine];
	int c = 0;
	int i = 0;
	int tabCoordsLeft = 0;

	for (; i < line.size() && c < aCoords.mColumn;)
	{
		if (line[i].mChar == '\t')
		{
			if (tabCoordsLeft == 0)
				tabCoordsLeft = TabSizeAtColumn(c);
			if (tabCoordsLeft > 0)
				tabCoordsLeft--;
			c++;
		}
		else
			++c;
		if (tabCoordsLeft == 0)
			i += UTF8CharLength(line[i].mChar);
	}
	return i;
}

int TextEditor::GetCharacterIndexR(const Coordinates& aCoords) const
{
	if (aCoords.mLine >= mLines.size())
		return -1;
	int c = 0;
	int i = 0;
	for (; i < mLines[aCoords.mLine].size() && c < aCoords.mColumn;)
		MoveCharIndexAndColumn(aCoords.mLine, i, c);
	return i;
}

int TextEditor::GetCharacterColumn(int aLine, int aIndex) const
{
	if (aLine >= mLines.size())
		return 0;
	int c = 0;
	int i = 0;
	while (i < aIndex && i < mLines[aLine].size())
		MoveCharIndexAndColumn(aLine, i, c);
	return c;
}

int TextEditor::GetFirstVisibleCharacterIndex(int aLine) const
{
	if (aLine >= mLines.size())
		return 0;
	int c = 0;
	int i = 0;
	while (c < mFirstVisibleColumn && i < mLines[aLine].size())
		MoveCharIndexAndColumn(aLine, i, c);
	if (c > mFirstVisibleColumn)
		i--;
	return i;
}

int TextEditor::GetLineMaxColumn(int aLine, int aLimit) const
{
	if (aLine >= mLines.size())
		return 0;

	int c = 0;
	if (aLimit == -1) {
		for (int i = 0; i < mLines[aLine].size(); )
			MoveCharIndexAndColumn(aLine, i, c);
	}
	else {
		for (int i = 0; i < mLines[aLine].size(); ) {
			MoveCharIndexAndColumn(aLine, i, c);
			if (c > aLimit)
				return aLimit;
		}
	}
	return c;
}

TextEditor::Line& TextEditor::InsertLine(int aIndex)
{
	auto& result = *mLines.insert(mLines.begin() + aIndex, Line());
	if (mCursor.mEnd.mLine >= aIndex) {
		SetCursorPosition({ mCursor.mEnd.mLine + 1, mCursor.mEnd.mColumn });
	}
	return result;
}

void TextEditor::RemoveLine(int aIndex)
{
	assert(mLines.size() > 1);

	mLines.erase(mLines.begin() + aIndex);
	assert(!mLines.empty());

	if (mCursor.mEnd.mLine >= aIndex) {
		SetCursorPosition({ mCursor.mEnd.mLine - 1, mCursor.mEnd.mColumn });
	}
}

void TextEditor::RemoveLines(int aStart, int aEnd)
{
	assert(aEnd >= aStart);
	assert(mLines.size() > (size_t)(aEnd - aStart));

	mLines.erase(mLines.begin() + aStart, mLines.begin() + aEnd);
	assert(!mLines.empty());

	if (mCursor.mEnd.mLine >= aStart) {
		int targetLine = mCursor.mEnd.mLine - (aEnd - aStart);
		targetLine = targetLine < 0 ? 0 : targetLine;
		SetCursorPosition({ targetLine , mCursor.mEnd.mColumn });
	}
}

void TextEditor::DeleteRange(const Coordinates& aStart, const Coordinates& aEnd)
{
	assert(aEnd >= aStart);
	if (aEnd == aStart)
		return;

	auto start = GetCharacterIndexL(aStart);
	auto end = GetCharacterIndexR(aEnd);

	if (aStart.mLine == aEnd.mLine) {
		auto n = GetLineMaxColumn(aStart.mLine);
		if (aEnd.mColumn >= n) {
			RemoveGlyphsFromLine(aStart.mLine, start);
		}
		else {
			RemoveGlyphsFromLine(aStart.mLine, start, end);
		}
	}
	else {
		RemoveGlyphsFromLine(aStart.mLine, start);
		RemoveGlyphsFromLine(aEnd.mLine, 0, end);
		auto& firstLine = mLines[aStart.mLine];
		auto& lastLine = mLines[aEnd.mLine];
		if (aStart.mLine < aEnd.mLine) {
			AddGlyphsToLine(aStart.mLine, (int) firstLine.size(), lastLine.begin(), lastLine.end());
			if (mCursor.mEnd.mLine <= aEnd.mLine && mCursor.mEnd.mLine == aEnd.mLine) {
				int otherCursorEndCharIndex = GetCharacterIndexR(mCursor.mEnd);
				int otherCursorStartCharIndex = GetCharacterIndexR(mCursor.mStart);
				int otherCursorNewEndCharIndex = GetCharacterIndexR(aStart) + otherCursorEndCharIndex;
				int otherCursorNewStartCharIndex = GetCharacterIndexR(aStart) + otherCursorStartCharIndex;
				auto targetEndCoords = Coordinates(aStart.mLine, GetCharacterColumn(aStart.mLine, otherCursorNewEndCharIndex));
				auto targetStartCoords = Coordinates(aStart.mLine, GetCharacterColumn(aStart.mLine, otherCursorNewStartCharIndex));
				SetCursorPosition(targetStartCoords, true);
				SetCursorPosition(targetEndCoords, false);
			}
			RemoveLines(aStart.mLine + 1, aEnd.mLine + 1);
		}
	}
}

void TextEditor::DeleteSelection()
{
	if (mCursor.GetSelectionEnd() == mCursor.GetSelectionStart())
		return;

	DeleteRange(mCursor.GetSelectionStart(), mCursor.GetSelectionEnd());
	SetCursorPosition(mCursor.GetSelectionStart());
	Colorize(mCursor.GetSelectionStart().mLine, 1);
}

void TextEditor::RemoveGlyphsFromLine(int aLine, int aStartChar, int aEndChar)
{
	auto& line = mLines[aLine];
	line.erase(line.begin() + aStartChar, aEndChar == -1 ? line.end() : line.begin() + aEndChar);
}

void TextEditor::AddGlyphsToLine(int aLine, int aTargetIndex, Line::iterator aSourceStart, Line::iterator aSourceEnd)
{
	auto& line = mLines[aLine];
	line.insert(line.begin() + aTargetIndex, aSourceStart, aSourceEnd);
}

void TextEditor::AddGlyphToLine(int aLine, int aTargetIndex, Glyph aGlyph)
{
	auto& line = mLines[aLine];
	line.insert(line.begin() + aTargetIndex, aGlyph);
}

ImU32 TextEditor::GetGlyphColor(const Glyph& aGlyph) const
{
	if (!mLanguageDefinition) {
		return PALETTE[(int) PaletteIndex::Default];
	}
	if (aGlyph.mComment) {
		return PALETTE[(int) PaletteIndex::Comment];
	}
	if (aGlyph.mMultiLineComment) {
		return PALETTE[(int) PaletteIndex::MultiLineComment];
	}
	auto const color = PALETTE[(int) aGlyph.mColorIndex];
	if (aGlyph.mPreprocessor) {
		const auto ppcolor = PALETTE[(int) PaletteIndex::Preprocessor];
		const int c0 = ((ppcolor & 0xff) + (color & 0xff)) / 2;
		const int c1 = (((ppcolor >> 8) & 0xff) + ((color >> 8) & 0xff)) / 2;
		const int c2 = (((ppcolor >> 16) & 0xff) + ((color >> 16) & 0xff)) / 2;
		const int c3 = (((ppcolor >> 24) & 0xff) + ((color >> 24) & 0xff)) / 2;
		return ImU32(c0 | (c1 << 8) | (c2 << 16) | (c3 << 24));
	}
	return color;
}

void TextEditor::HandleKeyboardInputs(bool aParentIsFocused)
{
	if (ImGui::IsWindowFocused() || aParentIsFocused) {
		if (ImGui::IsWindowHovered()) {
			ImGui::SetMouseCursor(ImGuiMouseCursor_TextInput);
		}
		//ImGui::CaptureKeyboardFromApp(true);

		ImGuiIO& io = ImGui::GetIO();
		auto isOSX = io.ConfigMacOSXBehaviors;
		auto alt = io.KeyAlt;
		auto ctrl = io.KeyCtrl;
		auto shift = io.KeyShift;
		auto super = io.KeySuper;

		auto isShortcut = (isOSX ? (super && !ctrl) : (ctrl && !super)) && !alt && !shift;
		auto isShiftShortcut = (isOSX ? (super && !ctrl) : (ctrl && !super)) && shift && !alt;
		auto isWordmoveKey = isOSX ? alt : ctrl;
		auto isAltOnly = alt && !ctrl && !shift && !super;
		auto isCtrlOnly = ctrl && !alt && !shift && !super;
		auto isShiftOnly = shift && !alt && !ctrl && !super;

		io.WantCaptureKeyboard = true;
		io.WantTextInput = true;

		if (isShortcut && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Z)))
			Undo();
		else if (isShortcut && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Y)))
			Redo();
		else if (!alt && !ctrl && !super && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_UpArrow)))
			MoveUp(1, shift);
		else if (!alt && !ctrl && !super && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_DownArrow)))
			MoveDown(1, shift);
		else if ((isOSX ? !ctrl : !alt) && !super && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_LeftArrow)))
			MoveLeft(shift, isWordmoveKey);
		else if ((isOSX ? !ctrl : !alt) && !super && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_RightArrow)))
			MoveRight(shift, isWordmoveKey);
		else if (!alt && !ctrl && !super && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_PageUp)))
			MoveUp(mVisibleLineCount - 2, shift);
		else if (!alt && !ctrl && !super && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_PageDown)))
			MoveDown(mVisibleLineCount - 2, shift);
		else if (ctrl && !alt && !super && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Home)))
			MoveTop(shift);
		else if (ctrl && !alt && !super && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_End)))
			MoveBottom(shift);
		else if (!alt && !ctrl && !super && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Home)))
			MoveHome(shift);
		else if (!alt && !ctrl && !super && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_End)))
			MoveEnd(shift);
		else if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete)))
			Delete(ctrl);
		else if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Backspace)))
			Backspace(ctrl);
		else if (!alt && ctrl && shift && !super && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_K)))
			RemoveCurrentLines();
		else if (!alt && ctrl && !shift && !super && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_LeftBracket)))
			ChangeCurrentLinesIndentation(false);
		else if (!alt && ctrl && !shift && !super && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_RightBracket)))
			ChangeCurrentLinesIndentation(true);
		else if (!alt && ctrl && shift && !super && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_UpArrow)))
			MoveUpCurrentLines();
		else if (!alt && ctrl && shift && !super && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_DownArrow)))
			MoveDownCurrentLines();
		else if (!alt && ctrl && !shift && !super && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Slash)))
			ToggleLineComment();
		else if (isCtrlOnly && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Insert)))
			Copy();
		else if (isShortcut && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_C)))
			Copy();
		else if (isShiftOnly && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Insert)))
			Paste();
		else if (isShortcut && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_V)))
			Paste();
		else if (isShortcut && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_X)))
			Cut();
		else if (isShiftOnly && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete)))
			Cut();
		else if (isShortcut && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_A)))
			SelectAll();
        else if (!alt && !ctrl && !shift && !super && (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)) || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_KeypadEnter))))
			EnterCharacter('\n', false);
		else if (!alt && !ctrl && !super && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Tab)))
			EnterCharacter('\t', shift);

		if (!io.InputQueueCharacters.empty() && ctrl == alt && !super) {
			for (int i = 0; i < io.InputQueueCharacters.Size; i++) {
				auto c = io.InputQueueCharacters[i];
				if (c != 0 && (c == '\n' || c >= 32))
					EnterCharacter(c, shift);
			}
			io.InputQueueCharacters.resize(0);
		}
	}
}

void TextEditor::HandleMouseInputs()
{
	ImGuiIO& io = ImGui::GetIO();
	auto shift = io.KeyShift;
	auto ctrl = io.ConfigMacOSXBehaviors ? io.KeySuper : io.KeyCtrl;
	auto alt = io.ConfigMacOSXBehaviors ? io.KeyCtrl : io.KeyAlt;

	/*
	Pan with middle mouse button
	*/
	mPanning &= ImGui::IsMouseDown(2);
	if (mPanning && ImGui::IsMouseDragging(2))
	{
		ImVec2 scroll = { ImGui::GetScrollX(), ImGui::GetScrollY() };
		ImVec2 currentMousePos = ImGui::GetMouseDragDelta(2);
		ImVec2 mouseDelta = {
			currentMousePos.x - mLastMousePos.x,
			currentMousePos.y - mLastMousePos.y
		};
		ImGui::SetScrollY(scroll.y - mouseDelta.y);
		ImGui::SetScrollX(scroll.x - mouseDelta.x);
		mLastMousePos = currentMousePos;
	}

	mDraggingSelection &= ImGui::IsMouseDown(0);
	if (mDraggingSelection && ImGui::IsMouseDragging(0)) {
		io.WantCaptureMouse = true;
		Coordinates cursorCoords = ScreenPosToCoordinates(ImGui::GetMousePos());
		SetCursorPosition(cursorCoords, false);
	}

	if (ImGui::IsWindowHovered()) {
		auto click = ImGui::IsMouseClicked(0);
		if (!shift && !alt) {
			auto doubleClick = ImGui::IsMouseDoubleClicked(0);
			auto t = ImGui::GetTime();
			auto tripleClick = click && !doubleClick &&
				(mLastClickTime != -1.0f && (t - mLastClickTime) < io.MouseDoubleClickTime &&
					Distance(io.MousePos, mLastClickPos) < 0.01f);

			if (click)
				mDraggingSelection = true;

			/*
			Pan with middle mouse button
			*/

			if (ImGui::IsMouseClicked(2))
			{
				mPanning = true;
				mLastMousePos = ImGui::GetMouseDragDelta(2);
			}

			/*
			Left mouse button triple click
			*/

			if (tripleClick) {
				Coordinates cursorCoords = ScreenPosToCoordinates(ImGui::GetMousePos());
				Coordinates targetCursorPos = cursorCoords.mLine < mLines.size() - 1 ?
					Coordinates{ cursorCoords.mLine + 1, 0 } :
					Coordinates{ cursorCoords.mLine, GetLineMaxColumn(cursorCoords.mLine) };
				SetSelection({ cursorCoords.mLine, 0 }, targetCursorPos);
				mLastClickTime = -1.0f;
			}

			/*
			Left mouse button double click
			*/

			else if (doubleClick) {
				Coordinates cursorCoords = ScreenPosToCoordinates(ImGui::GetMousePos());
				SetSelection(FindWordStart(cursorCoords), FindWordEnd(cursorCoords));
				mLastClickTime = (float)ImGui::GetTime();
				mLastClickPos = io.MousePos;
			}

			/*
			Left mouse button click
			*/
			else if (click) {
				bool isOverLineNumber;
				Coordinates cursorCoords = ScreenPosToCoordinates(ImGui::GetMousePos(), &isOverLineNumber);
				if (isOverLineNumber) {
					Coordinates targetCursorPos = cursorCoords.mLine < mLines.size() - 1 ?
						Coordinates{ cursorCoords.mLine + 1, 0 } :
						Coordinates{ cursorCoords.mLine, GetLineMaxColumn(cursorCoords.mLine) };
					SetSelection({ cursorCoords.mLine, 0 }, targetCursorPos);
				}
				else {
					SetCursorPosition(cursorCoords);
				}
				mLastClickTime = (float)ImGui::GetTime();
				mLastClickPos = io.MousePos;
			}
		}
		else if (shift) {
			if (click) {
				Coordinates newSelection = ScreenPosToCoordinates(ImGui::GetMousePos());
				SetCursorPosition(newSelection, false);
			}
		}
	}
}

void TextEditor::UpdateViewVariables(float aScrollX, float aScrollY)
{
	mContentHeight = ImGui::GetWindowHeight() - (IsHorizontalScrollbarVisible() ? IMGUI_SCROLLBAR_WIDTH : 0.0f);
	mContentWidth = ImGui::GetWindowWidth() - (IsVerticalScrollbarVisible() ? IMGUI_SCROLLBAR_WIDTH : 0.0f);

	mVisibleLineCount = std::max((int)ceil(mContentHeight / mCharAdvance.y), 0);
	mFirstVisibleLine = std::max((int)(aScrollY / mCharAdvance.y), 0);
	mLastVisibleLine = std::max((int)((mContentHeight + aScrollY) / mCharAdvance.y), 0);

	mVisibleColumnCount = std::max((int)ceil((mContentWidth - std::max(mTextStart - aScrollX, 0.0f)) / mCharAdvance.x), 0);
	mFirstVisibleColumn = std::max((int)(std::max(aScrollX - mTextStart, 0.0f) / mCharAdvance.x), 0);
	mLastVisibleColumn = std::max((int)((mContentWidth + aScrollX - mTextStart) / mCharAdvance.x), 0);
}

void TextEditor::Render(bool aParentIsFocused)
{
	/* Compute mCharAdvance regarding to scaled font size (Ctrl + mouse wheel)*/
	const float fontWidth = ImGui::GetFont()->CalcTextSizeA(ImGui::GetFontSize(), FLT_MAX, -1.0f, "#", nullptr, nullptr).x;
	const float fontHeight = ImGui::GetTextLineHeightWithSpacing();
	mCharAdvance = ImVec2(fontWidth, fontHeight * mLineSpacing);

	// Deduce mTextStart by evaluating mLines size (global lineMax) plus two spaces as text width
	mTextStart = (float) mLeftMargin;
	static char lineNumberBuffer[16];
	if (mShowLineNumbers)
	{
		snprintf(lineNumberBuffer, 16, " %d ", (int) mLines.size());
		mTextStart += ImGui::GetFont()->CalcTextSizeA(ImGui::GetFontSize(), FLT_MAX, -1.0f, lineNumberBuffer, nullptr, nullptr).x;
	}

	ImVec2 cursorScreenPos = ImGui::GetCursorScreenPos();
	mScrollX = ImGui::GetScrollX();
	mScrollY = ImGui::GetScrollY();
	UpdateViewVariables(mScrollX, mScrollY);

	int maxColumnLimited = 0;
	if (!mLines.empty())
	{
		auto drawList = ImGui::GetWindowDrawList();
		float spaceSize = ImGui::GetFont()->CalcTextSizeA(ImGui::GetFontSize(), FLT_MAX, -1.0f, " ", nullptr, nullptr).x;

		for (int lineNo = mFirstVisibleLine; lineNo <= mLastVisibleLine && lineNo < mLines.size(); lineNo++)
		{
			ImVec2 lineStartScreenPos = ImVec2(cursorScreenPos.x, cursorScreenPos.y + lineNo * mCharAdvance.y);
			ImVec2 textScreenPos = ImVec2(lineStartScreenPos.x + mTextStart, lineStartScreenPos.y);

			auto& line = mLines[lineNo];
			maxColumnLimited = std::max(GetLineMaxColumn(lineNo, mLastVisibleColumn), maxColumnLimited);

			Coordinates lineStartCoord(lineNo, 0);
			Coordinates lineEndCoord(lineNo, maxColumnLimited);

			// Draw selection for the current line
			float rectStart = -1.0f;
			float rectEnd = -1.0f;
			Coordinates cursorSelectionStart = mCursor.GetSelectionStart();
			Coordinates cursorSelectionEnd = mCursor.GetSelectionEnd();
			assert(cursorSelectionStart <= cursorSelectionEnd);

			if (cursorSelectionStart <= lineEndCoord)
				rectStart = cursorSelectionStart > lineStartCoord ? TextDistanceToLineStart(cursorSelectionStart) : 0.0f;
			if (cursorSelectionEnd > lineStartCoord)
				rectEnd = TextDistanceToLineStart(cursorSelectionEnd < lineEndCoord ? cursorSelectionEnd : lineEndCoord);
			if (cursorSelectionEnd.mLine > lineNo || cursorSelectionEnd.mLine == lineNo && cursorSelectionEnd > lineEndCoord)
				rectEnd += mCharAdvance.x;

			if (rectStart != -1 && rectEnd != -1 && rectStart < rectEnd)
				drawList->AddRectFilled(
					ImVec2{ lineStartScreenPos.x + mTextStart + rectStart, lineStartScreenPos.y },
					ImVec2{ lineStartScreenPos.x + mTextStart + rectEnd, lineStartScreenPos.y + mCharAdvance.y },
					PALETTE[(int) PaletteIndex::Selection]);

			// Draw line number (right aligned)
			if (mShowLineNumbers)
			{
				snprintf(lineNumberBuffer, 16, "%d  ", lineNo + 1);
				float lineNoWidth = ImGui::GetFont()->CalcTextSizeA(ImGui::GetFontSize(), FLT_MAX, -1.0f, lineNumberBuffer, nullptr, nullptr).x;
				drawList->AddText(ImVec2(lineStartScreenPos.x + mTextStart - lineNoWidth, lineStartScreenPos.y), PALETTE[(int)PaletteIndex::LineNumber], lineNumberBuffer);
			}

			std::vector<Coordinates> cursorCoordsInThisLine;
			if (mCursor.mEnd.mLine == lineNo)
				cursorCoordsInThisLine.push_back(mCursor.mEnd);

			if (cursorCoordsInThisLine.size() > 0 && ImGui::IsWindowFocused() || aParentIsFocused) {
				for (const auto& cursorCoords : cursorCoordsInThisLine) {
					float width = 1.0f;
					auto cindex = GetCharacterIndexR(cursorCoords);
					float cx = TextDistanceToLineStart(cursorCoords);

					ImVec2 cstart(textScreenPos.x + cx, lineStartScreenPos.y);
					ImVec2 cend(textScreenPos.x + cx + width, lineStartScreenPos.y + mCharAdvance.y);
					drawList->AddRectFilled(cstart, cend, PALETTE[(int) PaletteIndex::Cursor]);
					if (mCursorOnBracket) {
						ImVec2 topLeft = { cstart.x, lineStartScreenPos.y + fontHeight + 1.0f };
						ImVec2 bottomRight = { topLeft.x + mCharAdvance.x, topLeft.y + 1.0f };
						drawList->AddRectFilled(topLeft, bottomRight, PALETTE[(int) PaletteIndex::Cursor]);
					}
				}
			}

			// Render colorized text
			static std::string glyphBuffer;
			int charIndex = GetFirstVisibleCharacterIndex(lineNo);
			int column = mFirstVisibleColumn; // can be in the middle of tab character
			while (charIndex < mLines[lineNo].size() && column <= mLastVisibleColumn)
			{
				auto& glyph = line[charIndex];
				auto color = GetGlyphColor(glyph);
				ImVec2 targetGlyphPos = { lineStartScreenPos.x + mTextStart + TextDistanceToLineStart({lineNo, column}, false), lineStartScreenPos.y };

				if (glyph.mChar == '\t')
				{
					if (mShowWhitespaces)
					{
						ImVec2 p1, p2, p3, p4;

						const auto s = ImGui::GetFontSize();
						const auto x1 = targetGlyphPos.x + mCharAdvance.x * 0.3f;
						const auto y = targetGlyphPos.y + fontHeight * 0.5f;

						const auto x2 = targetGlyphPos.x + TabSizeAtColumn(column) * mCharAdvance.x - mCharAdvance.x * 0.3f;
						p1 = ImVec2(x1, y);
						p2 = ImVec2(x2, y);
						p3 = ImVec2(x2 - s * 0.2f, y - s * 0.2f);
						p4 = ImVec2(x2 - s * 0.2f, y + s * 0.2f);

						drawList->AddLine(p1, p2, PALETTE[(int)PaletteIndex::ControlCharacter]);
						drawList->AddLine(p2, p3, PALETTE[(int)PaletteIndex::ControlCharacter]);
						drawList->AddLine(p2, p4, PALETTE[(int)PaletteIndex::ControlCharacter]);
					}
				}
				else if (glyph.mChar == ' ')
				{
					if (mShowWhitespaces)
					{
						const auto s = ImGui::GetFontSize();
						const auto x = targetGlyphPos.x + spaceSize * 0.5f;
						const auto y = targetGlyphPos.y + s * 0.5f;
						drawList->AddCircleFilled(ImVec2(x, y), 1.5f, PALETTE[(int)PaletteIndex::ControlCharacter], 4);
					}
				}
				else
				{
					int seqLength = UTF8CharLength(glyph.mChar);
					if (mCursorOnBracket && seqLength == 1 && mMatchingBracketCoords == Coordinates{ lineNo, column })
					{
						ImVec2 topLeft = { targetGlyphPos.x, targetGlyphPos.y + fontHeight + 1.0f };
						ImVec2 bottomRight = { topLeft.x + mCharAdvance.x, topLeft.y + 1.0f };
						drawList->AddRectFilled(topLeft, bottomRight, PALETTE[(int)PaletteIndex::Cursor]);
					}
					glyphBuffer.clear();
					for (int i = 0; i < seqLength; i++)
						glyphBuffer.push_back(line[charIndex + i].mChar);
					drawList->AddText(targetGlyphPos, color, glyphBuffer.c_str());
				}

				MoveCharIndexAndColumn(lineNo, charIndex, column);
			}
		}
	}
	mCurrentSpaceHeight = (mLines.size() + std::min(mVisibleLineCount - 1, (int)mLines.size())) * mCharAdvance.y;
	mCurrentSpaceWidth = std::max((maxColumnLimited + std::min(mVisibleColumnCount - 1, maxColumnLimited)) * mCharAdvance.x, mCurrentSpaceWidth);

	ImGui::SetCursorPos(ImVec2(0, 0));
	ImGui::Dummy(ImVec2(mCurrentSpaceWidth, mCurrentSpaceHeight));

	if (mEnsureCursorVisible > -1)
	{
		for (int i = 0; i < (mEnsureCursorVisibleStartToo ? 2 : 1); i++) // first pass for interactive end and second pass for interactive start
		{
			if (i) UpdateViewVariables(mScrollX, mScrollY); // second pass depends on changes made in first pass
			Coordinates targetCoords = GetActualCursorCoordinates(i); // cursor selection end or start
			if (targetCoords.mLine <= mFirstVisibleLine)
			{
				float targetScroll = std::max(0.0f, (targetCoords.mLine - 0.5f) * mCharAdvance.y);
				if (targetScroll < mScrollY)
					ImGui::SetScrollY(targetScroll);
			}
			if (targetCoords.mLine >= mLastVisibleLine)
			{
				float targetScroll = std::max(0.0f, (targetCoords.mLine + 1.5f) * mCharAdvance.y - mContentHeight);
				if (targetScroll > mScrollY)
					ImGui::SetScrollY(targetScroll);
			}
			if (targetCoords.mColumn <= mFirstVisibleColumn)
			{
				float targetScroll = std::max(0.0f, mTextStart + (targetCoords.mColumn - 0.5f) * mCharAdvance.x);
				if (targetScroll < mScrollX)
					ImGui::SetScrollX(mScrollX = targetScroll);
			}
			if (targetCoords.mColumn >= mLastVisibleColumn)
			{
				float targetScroll = std::max(0.0f, mTextStart + (targetCoords.mColumn + 0.5f) * mCharAdvance.x - mContentWidth);
				if (targetScroll > mScrollX)
					ImGui::SetScrollX(mScrollX = targetScroll);
			}
		}
		mEnsureCursorVisible = -1;
	}
	if (mSetViewAtLine > -1)
	{
		float targetScroll;
		switch (mSetViewAtLineMode)
		{
		default:
		case SetViewAtLineMode::FirstVisibleLine:
			targetScroll = std::max(0.0f, (float)mSetViewAtLine * mCharAdvance.y);
			break;
		case SetViewAtLineMode::LastVisibleLine:
			targetScroll = std::max(0.0f, (float)(mSetViewAtLine - (mLastVisibleLine - mFirstVisibleLine)) * mCharAdvance.y);
			break;
		case SetViewAtLineMode::Centered:
			targetScroll = std::max(0.0f, ((float)mSetViewAtLine - (float)(mLastVisibleLine - mFirstVisibleLine) * 0.5f) * mCharAdvance.y);
			break;
		}
		ImGui::SetScrollY(targetScroll);
		mSetViewAtLine = -1;
	}
}

void TextEditor::DisplayPopup(const bool aParentIsFocused)
{

}

void TextEditor::OnCursorPositionChanged()
{
	if (mCursor.HasSelection()) {
		mCursorOnBracket = false;
		return;
	}
	mCursorOnBracket = FindMatchingBracket(mCursor.mEnd.mLine,
		GetCharacterIndexR(mCursor.mEnd), mMatchingBracketCoords);
}

void TextEditor::AddUndo(UndoRecord& aValue)
{
	mUndoBuffer.resize((size_t)(mUndoIndex + 1));
	mUndoBuffer.back() = aValue;
	++mUndoIndex;
}

// TODO
// - multiline comments vs single-line: latter is blocking start of a ML
void TextEditor::Colorize(int aFromLine, int aLines)
{
	int toLine = aLines == -1 ? (int)mLines.size() : std::min((int)mLines.size(), aFromLine + aLines);
	mColorRangeMin = std::min(mColorRangeMin, aFromLine);
	mColorRangeMax = std::max(mColorRangeMax, toLine);
	mColorRangeMin = std::max(0, mColorRangeMin);
	mColorRangeMax = std::max(mColorRangeMin, mColorRangeMax);
	mCheckComments = true;
}

void TextEditor::ColorizeRange(int aFromLine, int aToLine)
{
	if (mLines.empty() || aFromLine >= aToLine || mLanguageDefinition == nullptr)
		return;

	std::string buffer;
	std::string id;

	int endLine = std::max(0, std::min((int)mLines.size(), aToLine));
	for (int i = aFromLine; i < endLine; ++i) {
		auto& line = mLines[i];
		if (line.empty())
			continue;

		buffer.resize(line.size());
		for (size_t j = 0; j < line.size(); ++j) {
			auto& col = line[j];
			col.mColorIndex = PaletteIndex::Default;
			buffer[j] = col.mChar;
		}

		const char* bufferBegin = &buffer.front();
		const char* bufferEnd = bufferBegin + buffer.size();
		for (const char* current = bufferBegin; current < bufferEnd;)
		{
			const char* token_begin = nullptr;
			const char* token_end = nullptr;
			PaletteIndex token_color = PaletteIndex::Default;

			mLanguageDefinition->mTokenize(current, bufferEnd, token_begin, token_end, token_color);

			const size_t token_length = token_end - token_begin;
			if (token_color == PaletteIndex::Identifier) {
				id.assign(token_begin, token_end);
				if (!line[current - bufferBegin].mPreprocessor) {
					if (mLanguageDefinition->mKeywords.contains(id)) {
						token_color = PaletteIndex::Keyword;
					}
					else if (mLanguageDefinition->mTypes.contains(id)) {
						token_color = PaletteIndex::Type;
					}
					else if (mLanguageDefinition->mMembers.contains(id)) {
						token_color = PaletteIndex::Member;
					}
					else if (mLanguageDefinition->mFunctions.contains(id)) {
						token_color = PaletteIndex::Function;
					}
					else if (mLanguageDefinition->mPreprocIdentifiers.contains(id)) {
						token_color = PaletteIndex::PreprocIdentifier;
					}
				}
				else {
					if (mLanguageDefinition->mPreprocIdentifiers.contains(id))
						token_color = PaletteIndex::PreprocIdentifier;
				}
			}

			for (size_t j = 0; j < token_length; ++j)
				line[(token_begin - bufferBegin) + j].mColorIndex = token_color;

			current = token_end ? token_end : (current + 1);
		}
	}
}

template<class InputIt1, class InputIt2, class BinaryPredicate>
bool ColorizerEquals(InputIt1 first1, InputIt1 last1,
	InputIt2 first2, InputIt2 last2, BinaryPredicate p)
{
	for (; first1 != last1 && first2 != last2; ++first1, ++first2)
	{
		if (!p(*first1, *first2))
			return false;
	}
	return first1 == last1 && first2 == last2;
}
void TextEditor::ColorizeInternal()
{
	if (mLines.empty() || mLanguageDefinition == nullptr)
		return;

	if (mCheckComments)
	{
		auto endLine = mLines.size();
		auto endIndex = 0;
		auto commentStartLine = endLine;
		auto commentStartIndex = endIndex;
		auto withinString = false;
		auto withinSingleLineComment = false;
		auto withinPreproc = false;
		auto firstChar = true;			// there is no other non-whitespace characters in the line before
		auto concatenate = false;		// '\' on the very end of the line
		auto currentLine = 0;
		auto currentIndex = 0;
		while (currentLine < endLine || currentIndex < endIndex)
		{
			auto& line = mLines[currentLine];

			if (currentIndex == 0 && !concatenate)
			{
				withinSingleLineComment = false;
				withinPreproc = false;
				firstChar = true;
			}

			concatenate = false;

			if (!line.empty())
			{
				auto& g = line[currentIndex];
				auto c = g.mChar;

				if (c != mLanguageDefinition->mPreprocChar && !isspace(c))
					firstChar = false;

				if (currentIndex == (int)line.size() - 1 && line[line.size() - 1].mChar == '\\')
					concatenate = true;

				bool inComment = (commentStartLine < currentLine || (commentStartLine == currentLine && commentStartIndex <= currentIndex));

				if (withinString)
				{
					line[currentIndex].mMultiLineComment = inComment;

					if (c == '\"')
					{
						if (currentIndex + 1 < (int)line.size() && line[currentIndex + 1].mChar == '\"')
						{
							currentIndex += 1;
							if (currentIndex < (int)line.size())
								line[currentIndex].mMultiLineComment = inComment;
						}
						else
							withinString = false;
					}
					else if (c == '\\')
					{
						currentIndex += 1;
						if (currentIndex < (int)line.size())
							line[currentIndex].mMultiLineComment = inComment;
					}
				}
				else
				{
					if (firstChar && c == mLanguageDefinition->mPreprocChar)
						withinPreproc = true;

					if (c == '\"')
					{
						withinString = true;
						line[currentIndex].mMultiLineComment = inComment;
					}
					else
					{
						auto pred = [](const char& a, const Glyph& b) { return a == b.mChar; };
						auto from = line.begin() + currentIndex;
						auto& startStr = mLanguageDefinition->mCommentStart;
						auto& singleStartStr = mLanguageDefinition->mSingleLineComment;

						if (!withinSingleLineComment && currentIndex + startStr.size() <= line.size() &&
							ColorizerEquals(startStr.begin(), startStr.end(), from, from + startStr.size(), pred))
						{
							commentStartLine = currentLine;
							commentStartIndex = currentIndex;
						}
						else if (singleStartStr.size() > 0 &&
							currentIndex + singleStartStr.size() <= line.size() &&
							ColorizerEquals(singleStartStr.begin(), singleStartStr.end(), from, from + singleStartStr.size(), pred))
						{
							withinSingleLineComment = true;
						}

						inComment = (commentStartLine < currentLine || (commentStartLine == currentLine && commentStartIndex <= currentIndex));

						line[currentIndex].mMultiLineComment = inComment;
						line[currentIndex].mComment = withinSingleLineComment;

						auto& endStr = mLanguageDefinition->mCommentEnd;
						if (currentIndex + 1 >= (int)endStr.size() &&
							ColorizerEquals(endStr.begin(), endStr.end(), from + 1 - endStr.size(), from + 1, pred))
						{
							commentStartIndex = endIndex;
							commentStartLine = endLine;
						}
					}
				}
				if (currentIndex < (int)line.size())
					line[currentIndex].mPreprocessor = withinPreproc;
				currentIndex += UTF8CharLength(c);
				if (currentIndex >= (int)line.size())
				{
					currentIndex = 0;
					++currentLine;
				}
			}
			else
			{
				currentIndex = 0;
				++currentLine;
			}
		}
		mCheckComments = false;
	}

	if (mColorRangeMin < mColorRangeMax)
	{
		const int increment = (mLanguageDefinition->mTokenize == nullptr) ? 10 : 10000;
		const int to = std::min(mColorRangeMin + increment, mColorRangeMax);
		ColorizeRange(mColorRangeMin, to);
		mColorRangeMin = to;

		if (mColorRangeMax == mColorRangeMin)
		{
			mColorRangeMin = std::numeric_limits<int>::max();
			mColorRangeMax = 0;
		}
		return;
	}
}

const std::unordered_map<char, char> TextEditor::OPEN_TO_CLOSE_CHAR = {
	{'{', '}'},
	{'(' , ')'},
	{'[' , ']'}
};
const std::unordered_map<char, char> TextEditor::CLOSE_TO_OPEN_CHAR = {
	{'}', '{'},
	{')' , '('},
	{']' , '['}
};
