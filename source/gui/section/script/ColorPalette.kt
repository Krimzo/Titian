package gui.section.script

import imgui.extension.texteditor.TextEditor
import java.awt.Color

internal object ColorPalette {
    const val Default = 0
    const val Keyword = 1
    const val Number = 2
    const val String = 3
    const val CharLiteral = 4
    const val Punctuation = 5
    const val Preprocessor = 6
    const val Identifier = 7
    const val KnownIdentifier = 8
    const val PreprocIdentifier = 9
    const val Comment = 10
    const val MultiLineComment = 11
    const val Background = 12
    const val Cursor = 13
    const val Selection = 14
    const val ErrorMarker = 15
    const val Breakpoint = 16
    const val LineNumber = 17
    const val CurrentLineFill = 18
    const val CurrentLineFillInactive = 19
    const val CurrentLineEdge = 20
    const val MAX = 21
    fun convertColor(color: Color): Int {
        return Color(color.blue, color.green, color.red, color.alpha).rgb
    }

    fun getCustomDark(textEditor: TextEditor): IntArray {
        val result = textEditor.darkPalette
        result[Keyword] = convertColor(Color(240, 155, 120))
        result[Number] = convertColor(Color(200, 95, 95))
        result[String] = convertColor(Color(211, 158, 104))
        result[CharLiteral] = convertColor(Color(204, 170, 135))
        result[Punctuation] = convertColor(Color(225, 225, 225))
        result[Preprocessor] = convertColor(Color(224, 179, 215))
        result[Identifier] = convertColor(Color(210, 210, 210))
        result[KnownIdentifier] = convertColor(Color(105, 210, 190))
        result[PreprocIdentifier] = convertColor(Color(210, 178, 203))
        result[Comment] = convertColor(Color(128, 128, 128))
        result[MultiLineComment] = convertColor(Color(116, 116, 116))
        result[Background] = convertColor(Color(25, 25, 25))
        result[Cursor] = convertColor(Color(220, 220, 220))
        result[Selection] = convertColor(Color(70, 70, 70))
        result[ErrorMarker] = convertColor(Color(196, 57, 57))
        result[Breakpoint] = convertColor(Color(222, 73, 73))
        result[LineNumber] = convertColor(Color(210, 210, 210))
        result[CurrentLineFill] = convertColor(Color(51, 51, 51, 10))
        result[CurrentLineFillInactive] = convertColor(Color(51, 51, 51, 20))
        result[CurrentLineEdge] = convertColor(Color(89, 89, 89))
        return result
    }
}