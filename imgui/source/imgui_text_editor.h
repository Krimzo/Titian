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
	TextEditor();

	enum class SetViewAtLineMode
	{
		FirstVisibleLine, Centered, LastVisibleLine
	};

	void SelectAll();
	void SelectLine(int aLine);
	void SelectRegion(int aStartLine, int aStartChar, int aEndLine, int aEndChar);
	void SelectNextOccurrenceOf(const char* aText, int aTextSize, bool aCaseSensitive = true);
	void SelectAllOccurrencesOf(const char* aText, int aTextSize, bool aCaseSensitive = true);
	bool AnyCursorHasSelection() const;
	bool AllCursorsHaveSelection() const;
	void ClearExtraCursors();
	void ClearSelections();
	void SetCursorPosition(int aLine, int aCharIndex);

	inline void GetCursorPosition(int& outLine, int& outColumn) const
	{
		auto coords = GetActualCursorCoordinates();
		outLine = coords.mLine;
		outColumn = coords.mColumn;
	}

	int GetFirstVisibleLine();
	int GetLastVisibleLine();
	void SetViewAtLine(int aLine, SetViewAtLineMode aMode);

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

	bool Render(const char* aTitle, bool aParentIsFocused = false, const ImVec2& aSize = ImVec2(), bool aBorder = false);

	inline std::string get_word_at_cursor() const
	{
		Coordinates cursor_coords{};
		this->GetCursorPosition(cursor_coords.mLine, cursor_coords.mColumn);
		if (cursor_coords.mColumn > 0) {
			cursor_coords.mColumn -= 1;
		}
		const Coordinates start = this->FindWordStart(cursor_coords);
		const Coordinates end = this->FindWordEnd(cursor_coords);
		if (start >= end) {
			return "";
		}
		return this->GetText(start, end);
	}

	inline void replace_word_at_cursor(const std::string_view& text)
	{
		Coordinates cursor_coords{};
		this->GetCursorPosition(cursor_coords.mLine, cursor_coords.mColumn);
		if (cursor_coords.mColumn > 0) {
			cursor_coords.mColumn -= 1;
		}

		Coordinates word_start = this->FindWordStart(cursor_coords);
		Coordinates word_end = this->FindWordEnd(cursor_coords);
		const int word_start_index = this->GetCharacterIndexL(word_start);
		const int word_end_index = this->GetCharacterIndexL(word_end);
		if (word_start_index < word_end_index) {
			RemoveGlyphsFromLine(cursor_coords.mLine, word_start_index, word_end_index);
		}
		this->InsertTextAt(word_start, text.data());
		this->Colorize(cursor_coords.mLine, 1);
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
	template<typename T>
	static inline T Max(T a, T b) { return a > b ? a : b; }
	template<typename T>
	static inline T Min(T a, T b) { return a < b ? a : b; }

	// ------------- Internal ------------- //

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

	// Represents a character coordinate from the user's point of view,
	// i. e. consider an uniform grid (assuming fixed-width font) on the
	// screen as it is rendered, and each cell has its own coordinate, starting from 0.
	// Tabs are counted as [1..mTabSize] count empty spaces, depending on
	// how many space is necessary to reach the next tab stop.
	// For example, coordinate (1, 5) represents the character 'B' in a line "\tABC", when mTabSize = 4,
	// because it is rendered as "    ABC" on the screen.
	struct Coordinates
	{
		int mLine, mColumn;
		Coordinates() : mLine(0), mColumn(0) {}
		Coordinates(int aLine, int aColumn) : mLine(aLine), mColumn(aColumn)
		{
			assert(aLine >= 0);
			assert(aColumn >= 0);
		}
		static Coordinates Invalid() { static Coordinates invalid(-1, -1); return invalid; }

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
	};

	struct Cursor
	{
		Coordinates mInteractiveStart = { 0, 0 };
		Coordinates mInteractiveEnd = { 0, 0 };
		inline Coordinates GetSelectionStart() const { return mInteractiveStart < mInteractiveEnd ? mInteractiveStart : mInteractiveEnd; }
		inline Coordinates GetSelectionEnd() const { return mInteractiveStart > mInteractiveEnd ? mInteractiveStart : mInteractiveEnd; }
		inline bool HasSelection() const { return mInteractiveStart != mInteractiveEnd; }
	};

	struct EditorState // state to be restored with undo/redo
	{
		int mCurrentCursor = 0;
		int mLastAddedCursor = 0;
		std::vector<Cursor> mCursors = { {{0,0}} };
		void AddCursor();
		int GetLastAddedCursorIndex();
		void SortCursorsFromTopToBottom();
	};

	struct Glyph
	{
		char mChar;
		PaletteIndex mColorIndex = PaletteIndex::Default;
		bool mComment : 1 = false;
		bool mMultiLineComment : 1 = false;
		bool mPreprocessor : 1 = false;

		Glyph(char aChar, PaletteIndex aColorIndex)
			: mChar(aChar), mColorIndex(aColorIndex)
		{}
	};

	typedef std::vector<Glyph> Line;

	struct LanguageDefinition
	{
		typedef void(*TokenizeCallback)(const char* in_begin, const char* in_end, const char*& out_begin, const char*& out_end, PaletteIndex& paletteIndex);

		std::string mName;
		std::set<std::string> mKeywords;
		std::map<std::string, Coordinates> mTypes;
		std::map<std::string, Coordinates> mMembers;
		std::map<std::string, Coordinates> mFunctions;
		std::map<std::string, Coordinates> mPreprocIdentifiers;
		std::string mCommentStart, mCommentEnd, mSingleLineComment;
		char mPreprocChar = '#';

		TokenizeCallback mTokenize = nullptr;

		static std::shared_ptr<LanguageDefinition> lua(
			const std::set<std::string, std::less<>>& keywords,
			const std::set<std::string, std::less<>>& types,
			const std::set<std::string, std::less<>>& members,
			const std::set<std::string, std::less<>>& functions);

		static std::shared_ptr<LanguageDefinition> hlsl(
			const std::set<std::string, std::less<>>& keywords,
			const std::set<std::string, std::less<>>& types,
			const std::set<std::string, std::less<>>& members,
			const std::set<std::string, std::less<>>& functions);
	};

	enum class UndoOperationType { Add, Delete };
	struct UndoOperation
	{
		std::string mText;
		TextEditor::Coordinates mStart;
		TextEditor::Coordinates mEnd;
		UndoOperationType mType;
	};

	class UndoRecord
	{
	public:
		UndoRecord() {}
		~UndoRecord() {}

		UndoRecord(
			const std::vector<UndoOperation>& aOperations,
			TextEditor::EditorState& aBefore,
			TextEditor::EditorState& aAfter);

		void Undo(TextEditor* aEditor);
		void Redo(TextEditor* aEditor);

		std::vector<UndoOperation> mOperations;

		EditorState mBefore;
		EditorState mAfter;
	};

	void SetLanguageDefinition(std::shared_ptr<LanguageDefinition> aValue);

	std::string GetText(const Coordinates& aStart, const Coordinates& aEnd) const;
	std::string GetClipboardText() const;
	std::string GetSelectedText(int aCursor = -1) const;

	void SetCursorPosition(const Coordinates& aPosition, int aCursor = -1, bool aClearSelection = true);

	int InsertTextAt(Coordinates& aWhere, const char* aValue);
	void InsertTextAtCursor(const std::string& aValue, int aCursor = -1);
	void InsertTextAtCursor(const char* aValue, int aCursor = -1);

	enum class MoveDirection { Right = 0, Left = 1, Up = 2, Down = 3 };
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
	void Delete(bool aWordMode = false, const EditorState* aEditorState = nullptr);

	void SetSelection(Coordinates aStart, Coordinates aEnd, int aCursor = -1);
	void SetSelection(int aStartLine, int aStartChar, int aEndLine, int aEndChar, int aCursor = -1);

	void SelectNextOccurrenceOf(const char* aText, int aTextSize, int aCursor = -1, bool aCaseSensitive = true);
	void AddCursorForNextOccurrence(bool aCaseSensitive = true);
	bool FindNextOccurrence(const char* aText, int aTextSize, const Coordinates& aFrom, Coordinates& outStart, Coordinates& outEnd, bool aCaseSensitive = true);
	bool FindMatchingBracket(int aLine, int aCharIndex, Coordinates& out);
	void ChangeCurrentLinesIndentation(bool aIncrease);
	void MoveUpCurrentLines();
	void MoveDownCurrentLines();
	void ToggleLineComment();
	void RemoveCurrentLines();

	float TextDistanceToLineStart(const Coordinates& aFrom, bool aSanitizeCoords = true) const;
	void EnsureCursorVisible(int aCursor = -1, bool aStartToo = false);

	Coordinates SanitizeCoordinates(const Coordinates& aValue) const;
	Coordinates GetActualCursorCoordinates(int aCursor = -1, bool aStart = false) const;
	Coordinates ScreenPosToCoordinates(const ImVec2& aPosition, bool* isOverLineNumber = nullptr) const;
	Coordinates FindWordStart(const Coordinates& aFrom) const;
	Coordinates FindWordEnd(const Coordinates& aFrom) const;
	int GetCharacterIndexL(const Coordinates& aCoordinates) const;
	int GetCharacterIndexR(const Coordinates& aCoordinates) const;
	int GetCharacterColumn(int aLine, int aIndex) const;
	int GetFirstVisibleCharacterIndex(int aLine) const;
	int GetLineMaxColumn(int aLine, int aLimit = -1) const;

	Line& InsertLine(int aIndex);
	void RemoveLine(int aIndex, const std::unordered_set<int>* aHandledCursors = nullptr);
	void RemoveLines(int aStart, int aEnd);
	void DeleteRange(const Coordinates& aStart, const Coordinates& aEnd);
	void DeleteSelection(int aCursor = -1);

	void RemoveGlyphsFromLine(int aLine, int aStartChar, int aEndChar = -1);
	void AddGlyphsToLine(int aLine, int aTargetIndex, Line::iterator aSourceStart, Line::iterator aSourceEnd);
	void AddGlyphToLine(int aLine, int aTargetIndex, Glyph aGlyph);
	ImU32 GetGlyphColor(const Glyph& aGlyph) const;

	void HandleKeyboardInputs(bool aParentIsFocused = false);
	void HandleMouseInputs();
	void UpdateViewVariables(float aScrollX, float aScrollY);
	void Render(bool aParentIsFocused = false);

	void OnCursorPositionChanged();
	void OnLineChanged(bool aBeforeChange, int aLine, int aColumn, int aCharCount, bool aDeleted);
	void MergeCursorsIfPossible();

	void AddUndo(UndoRecord& aValue);

	void Colorize(int aFromLine = 0, int aCount = -1);
	void ColorizeRange(int aFromLine = 0, int aToLine = 0);
	void ColorizeInternal();

	std::vector<Line> mLines;
	EditorState mState;
	std::vector<UndoRecord> mUndoBuffer;
	int mUndoIndex = 0;

	int mTabSize = 4;
	float mLineSpacing = 1.0f;
	bool mShowWhitespaces = true;
	bool mShowLineNumbers = true;

	int mSetViewAtLine = -1;
	SetViewAtLineMode mSetViewAtLineMode = {};
	int mEnsureCursorVisible = -1;
	bool mEnsureCursorVisibleStartToo = false;
	bool mScrollToTop = false;

	float mTextStart = 20.0f; // position (in pixels) where a code line starts relative to the left of the TextEditor.
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

	inline bool IsHorizontalScrollbarVisible() const { return mCurrentSpaceWidth > mContentWidth; }
	inline bool IsVerticalScrollbarVisible() const { return mCurrentSpaceHeight > mContentHeight; }
	inline int TabSizeAtColumn(int aColumn) const { return mTabSize - (aColumn % mTabSize); }

	static const std::unordered_map<char, char> OPEN_TO_CLOSE_CHAR;
	static const std::unordered_map<char, char> CLOSE_TO_OPEN_CHAR;
};
