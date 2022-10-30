package gui.section.script_editor;

import imgui.extension.texteditor.TextEditor;

import java.awt.*;

final class ColorPalette {
    private ColorPalette() {}

    static final int Default = 0;
    static final int Keyword = 1;
    static final int Number = 2;
    static final int String = 3;
    static final int CharLiteral = 4;
    static final int Punctuation = 5;
    static final int Preprocessor = 6;
    static final int Identifier = 7;
    static final int KnownIdentifier = 8;
    static final int PreprocIdentifier = 9;
    static final int Comment = 10;
    static final int MultiLineComment = 11;
    static final int Background = 12;
    static final int Cursor = 13;
    static final int Selection = 14;
    static final int ErrorMarker = 15;
    static final int Breakpoint = 16;
    static final int LineNumber = 17;
    static final int CurrentLineFill = 18;
    static final int CurrentLineFillInactive = 19;
    static final int CurrentLineEdge = 20;

    static final int MAX = 21;

    static int convertColor(Color color) {
        return new Color(color.getBlue(), color.getGreen(), color.getRed(), color.getAlpha()).getRGB();
    }

    static int[] getCustomDark(TextEditor textEditor) {
        final int[] result = textEditor.getDarkPalette();
        result[Keyword] = convertColor(new Color(240, 155, 120));
        result[Number] = convertColor(new Color(200, 95, 95));
        result[String] = convertColor(new Color(211, 158, 104));
        result[CharLiteral] = convertColor(new Color(204, 170, 135));
        result[Punctuation] = convertColor(new Color(225, 225, 225));
        result[Preprocessor] = convertColor(new Color(224, 179, 215));
        result[Identifier] = convertColor(new Color(210, 210, 210));
        result[KnownIdentifier] = convertColor(new Color(105, 210, 190));
        result[PreprocIdentifier] = convertColor(new Color(210, 178, 203));
        result[Comment] = convertColor(new Color(128, 128, 128));
        result[MultiLineComment] = convertColor(new Color(116, 116, 116));
        result[Background] = convertColor(new Color(25, 25, 25));
        result[Cursor] = convertColor(new Color(220, 220, 220));
        result[Selection] = convertColor(new Color(70, 70, 70));
        result[ErrorMarker] = convertColor(new Color(196, 57, 57));
        result[Breakpoint] = convertColor(new Color(222, 73, 73));
        result[LineNumber] = convertColor(new Color(210, 210, 210));
        result[CurrentLineFill] = convertColor(new Color(51, 51, 51, 10));
        result[CurrentLineFillInactive] = convertColor(new Color(51, 51, 51, 20));
        result[CurrentLineEdge] = convertColor(new Color(89, 89, 89));
        return result;
    }
}
