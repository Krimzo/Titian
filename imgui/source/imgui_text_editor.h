#pragma once

#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <map>
#include "imgui.h"


class IMGUI_API TextEditor
{
public:
	enum class SetViewAtLineMode : int8_t
	{
		FirstVisibleLine,
		Centered,
		LastVisibleLine,
	};

	enum class PaletteIndex : int8_t
	{
		Default,
		Keyword,
		Number,
		String,
		CharLiteral,
		Punctuation,
		Preprocessor,
		Identifier,
		Type,
		Member,
		Function,
		PreprocIdentifier,
		Comment,
		MultiLineComment,
		Background,
		Cursor,
		Selection,
		ErrorMarker,
		ControlCharacter,
		Breakpoint,
		LineNumber,
		CurrentLineFill,
		CurrentLineFillInactive,
		CurrentLineEdge,
		Max,
	};

	enum class MoveDirection : int8_t
	{
		Right = 0,
		Left = 1,
		Up = 2,
		Down = 3,
	};

	enum class UndoOperationType : int8_t
	{
		Add,
		Delete,
	};

	struct Coordinates
	{
		int mLine = 0, mColumn = 0;

		Coordinates() = default;

		inline Coordinates(int aLine, int aColumn)
			: mLine(aLine), mColumn(aColumn)
		{
			assert(aLine >= 0);
			assert(aColumn >= 0);
		}

		bool operator ==(const Coordinates& o) const
		{
			return
				mLine == o.mLine &&
				mColumn == o.mColumn;
		}

		bool operator !=(const Coordinates& o) const
		{
			return
				mLine != o.mLine ||
				mColumn != o.mColumn;
		}

		bool operator <(const Coordinates& o) const
		{
			if (mLine != o.mLine)
				return mLine < o.mLine;
			return mColumn < o.mColumn;
		}

		bool operator >(const Coordinates& o) const
		{
			if (mLine != o.mLine)
				return mLine > o.mLine;
			return mColumn > o.mColumn;
		}

		bool operator <=(const Coordinates& o) const
		{
			if (mLine != o.mLine)
				return mLine < o.mLine;
			return mColumn <= o.mColumn;
		}

		bool operator >=(const Coordinates& o) const
		{
			if (mLine != o.mLine)
				return mLine > o.mLine;
			return mColumn >= o.mColumn;
		}

		Coordinates operator -(const Coordinates& o)
		{
			return Coordinates(mLine - o.mLine, mColumn - o.mColumn);
		}

		Coordinates operator +(const Coordinates& o)
		{
			return Coordinates(mLine + o.mLine, mColumn + o.mColumn);
		}

		static Coordinates Invalid()
		{
			return { -1, -1 };
		}
	};

	struct Cursor
	{
		Coordinates mStart = { 0, 0 };
		Coordinates mEnd = { 0, 0 };

		Cursor() = default;

		inline Coordinates GetSelectionStart() const { return mStart < mEnd ? mStart : mEnd; }
		inline Coordinates GetSelectionEnd() const { return mStart > mEnd ? mStart : mEnd; }
		inline bool HasSelection() const { return mStart != mEnd; }
	};

	struct Glyph
	{
		char mChar;
		PaletteIndex mColorIndex = PaletteIndex::Default;
		bool mComment : 1 = false;
		bool mMultiLineComment : 1 = false;
		bool mPreprocessor : 1 = false;

		inline Glyph(char aChar, PaletteIndex aColorIndex)
			: mChar(aChar), mColorIndex(aColorIndex)
		{}
	};

	using Line = std::vector<Glyph>;

	struct LanguageDefinition
	{
		std::string mName;
		std::set<std::string> mKeywords;
		std::set<std::string> mTypes;
		std::set<std::string> mMembers;
		std::set<std::string> mFunctions;
		std::set<std::string> mPreprocIdentifiers;
		std::string mCommentStart, mCommentEnd, mSingleLineComment;
		char mPreprocChar = '#';

		using TokenizeCallback = void(*)(const char* in_begin, const char* in_end, const char*& out_begin, const char*& out_end, PaletteIndex& paletteIndex);
		TokenizeCallback mTokenize = nullptr;

		inline bool can_contain_word(const std::string& word) const
		{
			for (const auto& name : mKeywords) {
				if (name.find(word) != -1) {
					return true;
				}
			}
			for (const auto& name : mTypes) {
				if (name.find(word) != -1) {
					return true;
				}
			}
			for (const auto& name : mMembers) {
				if (name.find(word) != -1) {
					return true;
				}
			}
			for (const auto& name : mFunctions) {
				if (name.find(word) != -1) {
					return true;
				}
			}
			return false;
		}

		static std::shared_ptr<LanguageDefinition> lua(
			const std::set<std::string>& keywords,
			const std::set<std::string>& types,
			const std::set<std::string>& members,
			const std::set<std::string>& functions);

		static std::shared_ptr<LanguageDefinition> hlsl(
			const std::set<std::string>& keywords,
			const std::set<std::string>& types,
			const std::set<std::string>& members,
			const std::set<std::string>& functions);
	};

	struct UndoOperation
	{
		std::string mText;
		TextEditor::Coordinates mStart;
		TextEditor::Coordinates mEnd;
		UndoOperationType mType;
	};

	struct UndoRecord
	{
		std::vector<UndoOperation> mOperations;
		Cursor mBefore;
		Cursor mAfter;

		UndoRecord() = default;
		UndoRecord(const std::vector<UndoOperation>& aOperations, const Cursor& aBefore, const Cursor& aAfter);

		void Undo(TextEditor* aEditor);
		void Redo(TextEditor* aEditor);
	};

	static constexpr std::array<ImU32, (int) PaletteIndex::Max> PALETTE
	{
		IM_COL32(220, 220, 220, 255), // Default
		IM_COL32(216, 160, 223, 255), // Keyword
		IM_COL32(181, 206, 168, 255), // Number
		IM_COL32(232, 201, 187, 255), // String
		IM_COL32(232, 201, 187, 255), // Char literal
		IM_COL32(225, 225, 225, 255), // Punctuation
		IM_COL32(190, 183, 255, 255), // Preprocessor
		IM_COL32(210, 210, 210, 255), // Identifier
		IM_COL32( 86, 156, 214, 255), // Type
		IM_COL32(220, 205, 125, 255), // Member
		IM_COL32(185, 215, 160, 255), // Function
		IM_COL32(190, 183, 255, 255), // Preproc identifier
		IM_COL32( 87, 166,  74, 255), // Comment (single line)
		IM_COL32( 87, 166,  74, 255), // Comment (multi line)
		IM_COL32( 30,  30,  30, 255), // Background
		IM_COL32(220, 220, 220, 255), // Cursor
		IM_COL32( 70,  70,  70, 255), // Selection
		IM_COL32(196,  57,  57, 255), // ErrorMarker
		IM_COL32(255, 255, 255,  20), // ControlCharacter
		IM_COL32(222,  73,  73, 255), // Breakpoint
		IM_COL32(175, 175, 175, 255), // Line number
		IM_COL32( 51,  51,  51,  10), // Current line fill
		IM_COL32( 51,  51,  51,  20), // Current line fill (inactive)
		IM_COL32( 89,  89,  89, 255), // Current line edge
	};

	TextEditor() = default;

	void SetLanguageDefinition(std::shared_ptr<LanguageDefinition> aValue);

	void SelectAll();
	void SelectLine(int aLine);
	void SelectRegion(int aStartLine, int aStartChar, int aEndLine, int aEndChar);
	void ClearSelections();
	void SetCursorPosition(int aLine, int aCharIndex);

	void Copy();
	void Cut();
	void Paste();
	void Undo(int aSteps = 1);
	void Redo(int aSteps = 1);

	inline bool CanUndo() const { return mUndoIndex > 0; };
	inline bool CanRedo() const { return mUndoIndex < (int)mUndoBuffer.size(); };
	inline int GetUndoIndex() const { return mUndoIndex; };

	void SetText(const std::string_view& aText);
	std::string GetText() const;

	void SetTextLines(const std::vector<std::string>& aLines);
	std::vector<std::string> GetTextLines() const;

	bool Update(const char* aTitle, bool aParentIsFocused = false, const ImVec2& aSize = ImVec2(), bool aBorder = false);

	inline std::string get_word_at_cursor() const
	{
		Coordinates cursor_coords = GetActualCursorCoordinates();
		if (cursor_coords.mColumn > 0) {
			cursor_coords.mColumn -= 1;
		}
		const Coordinates start = FindWordStart(cursor_coords);
		const Coordinates end = FindWordEnd(cursor_coords);
		if (start >= end) {
			return "";
		}
		return GetText(start, end);
	}

	inline void replace_word_at_cursor(const std::string_view& text)
	{
		Coordinates cursor_coords = GetActualCursorCoordinates();
		if (cursor_coords.mColumn > 0) {
			cursor_coords.mColumn -= 1;
		}
		Coordinates word_start = FindWordStart(cursor_coords);
		const Coordinates word_end = FindWordEnd(cursor_coords);
		const int word_start_index = GetCharacterIndexL(word_start);
		const int word_end_index = GetCharacterIndexL(word_end);
		if (word_start_index < word_end_index) {
			RemoveGlyphsFromLine(cursor_coords.mLine, word_start_index, word_end_index);
		}
		InsertTextAt(word_start, text.data());
		Colorize(cursor_coords.mLine, 1);
		mCursor.mEnd = FindWordEnd(cursor_coords);
		mCursor.mStart = mCursor.mEnd;
	}

	static inline ImVec4 U32ColorToVec4(ImU32 in)
	{
		float s = 1.0f / 255.0f;
		return ImVec4(
			((in >> IM_COL32_A_SHIFT) & 0xFF) * s,
			((in >> IM_COL32_B_SHIFT) & 0xFF) * s,
			((in >> IM_COL32_G_SHIFT) & 0xFF) * s,
			((in >> IM_COL32_R_SHIFT) & 0xFF) * s);
	}

	static inline bool IsUTFSequence(char c)
	{
		return (c & 0xC0) == 0x80;
	}

	static inline float Distance(const ImVec2& a, const ImVec2& b)
	{
		float x = a.x - b.x;
		float y = a.y - b.y;
		return sqrt(x * x + y * y);
	}

	std::string GetText(const Coordinates& aStart, const Coordinates& aEnd) const;
	std::string GetClipboardText() const;
	std::string GetSelectedText() const;

	void SetCursorPosition(const Coordinates& aPosition, bool aClearSelection = true);

	int InsertTextAt(Coordinates& aWhere, const char* aValue);
	void InsertTextAtCursor(const std::string& aValue);
	void InsertTextAtCursor(const char* aValue);

	bool Move(int& aLine, int& aCharIndex, bool aLeft = false, bool aLockLine = false) const;
	void MoveCharIndexAndColumn(int aLine, int& aCharIndex, int& aColumn) const;
	void MoveCoords(Coordinates& aCoords, MoveDirection aDirection, bool aWordMode = false, int aLineCount = 1) const;

	void MoveUp(int aAmount = 1, bool aSelect = false);
	void MoveDown(int aAmount = 1, bool aSelect = false);
	void MoveLeft(bool aSelect = false, bool aWordMode = false);
	void MoveRight(bool aSelect = false, bool aWordMode = false);
	void MoveTop(bool aSelect = false);
	void MoveBottom(bool aSelect = false);
	void MoveHome(bool aSelect = false);
	void MoveEnd(bool aSelect = false);
	void EnterCharacter(ImWchar aChar, bool aShift);
	void Backspace(bool aWordMode = false);
	void Delete(bool aWordMode = false, const Cursor* aEditorState = nullptr);

	void SetSelection(Coordinates aStart, Coordinates aEnd);
	void SetSelection(int aStartLine, int aStartChar, int aEndLine, int aEndChar);

	bool FindMatchingBracket(int aLine, int aCharIndex, Coordinates& out);
	void ChangeCurrentLinesIndentation(bool aIncrease);
	void MoveUpCurrentLines();
	void MoveDownCurrentLines();
	void ToggleLineComment();
	void RemoveCurrentLines();

	float TextDistanceToLineStart(const Coordinates& aFrom, bool aSanitizeCoords = true) const;
	void EnsureCursorVisible(bool aStartToo = false);

	Coordinates SanitizeCoordinates(const Coordinates& aValue) const;
	Coordinates GetActualCursorCoordinates(bool aStart = false) const;
	Coordinates ScreenPosToCoordinates(const ImVec2& aPosition, bool* isOverLineNumber = nullptr) const;
	Coordinates FindWordStart(const Coordinates& aFrom) const;
	Coordinates FindWordEnd(const Coordinates& aFrom) const;
	int GetCharacterIndexL(const Coordinates& aCoordinates) const;
	int GetCharacterIndexR(const Coordinates& aCoordinates) const;
	int GetCharacterColumn(int aLine, int aIndex) const;
	int GetFirstVisibleCharacterIndex(int aLine) const;
	int GetLineMaxColumn(int aLine, int aLimit = -1) const;

	Line& InsertLine(int aIndex);
	void RemoveLine(int aIndex);
	void RemoveLines(int aStart, int aEnd);
	void DeleteRange(const Coordinates& aStart, const Coordinates& aEnd);
	void DeleteSelection();

	void RemoveGlyphsFromLine(int aLine, int aStartChar, int aEndChar = -1);
	void AddGlyphsToLine(int aLine, int aTargetIndex, Line::iterator aSourceStart, Line::iterator aSourceEnd);
	void AddGlyphToLine(int aLine, int aTargetIndex, Glyph aGlyph);
	ImU32 GetGlyphColor(const Glyph& aGlyph) const;

	void HandleKeyboardInputs(bool aParentIsFocused = false);
	void HandleMouseInputs();
	void UpdateViewVariables(float aScrollX, float aScrollY);
	void RenderEditor(bool aParentIsFocused = false);
	void DisplayPopup(bool aParentIsFocused);

	void OnCursorPositionChanged();

	void AddUndo(UndoRecord& aValue);

	void Colorize(int aFromLine = 0, int aCount = -1);
	void ColorizeRange(int aFromLine = 0, int aToLine = 0);
	void ColorizeInternal();

	inline bool IsHorizontalScrollbarVisible() const { return mCurrentSpaceWidth > mContentWidth; }
	inline bool IsVerticalScrollbarVisible() const { return mCurrentSpaceHeight > mContentHeight; }
	inline int TabSizeAtColumn(int aColumn) const { return mTabSize - (aColumn % mTabSize); }

	std::vector<Line> mLines{ Line{} };
	Cursor mCursor{};

	std::vector<UndoRecord> mUndoBuffer{};
	int mUndoIndex = 0;

	int mTabSize = 4;
	float mLineSpacing = 1.0f;
	bool mShowWhitespaces = true;
	bool mShowLineNumbers = true;

	bool mShowPopup = false;
	int mPopupIndex = -1;
	ImVec2 mPopupPos{};

	int mSetViewAtLine = -1;
	SetViewAtLineMode mSetViewAtLineMode = {};
	int mEnsureCursorVisible = -1;
	bool mEnsureCursorVisibleStartToo = false;

	float mTextStart = 20.0f;
	int mLeftMargin = 10;
	ImVec2 mCharAdvance;
	float mCurrentSpaceHeight = 20.0f;
	float mCurrentSpaceWidth = 20.0f;
	float mLastClickTime = -1.0f;
	ImVec2 mLastClickPos;
	int mFirstVisibleLine = 0;
	int mLastVisibleLine = 0;
	int mVisibleLineCount = 0;
	int mFirstVisibleColumn = 0;
	int mLastVisibleColumn = 0;
	int mVisibleColumnCount = 0;
	float mContentWidth = 0.0f;
	float mContentHeight = 0.0f;
	float mScrollX = 0.0f;
	float mScrollY = 0.0f;
	bool mPanning = false;
	bool mDraggingSelection = false;
	ImVec2 mLastMousePos;
	bool mCursorPositionChanged = false;
	bool mCursorOnBracket = false;
	Coordinates mMatchingBracketCoords;

	int mColorRangeMin = 0;
	int mColorRangeMax = 0;
	bool mCheckComments = true;
	std::shared_ptr<LanguageDefinition> mLanguageDefinition;

	static const std::unordered_map<char, char> OPEN_TO_CLOSE_CHAR;
	static const std::unordered_map<char, char> CLOSE_TO_OPEN_CHAR;
};
