package gui;

import gui.helper.GUIFonts;
import imgui.ImFont;

public final class GUIFontData {
    public final ImFont defaultFont;
    public final ImFont scriptEditorFont;

    public GUIFontData() {
        defaultFont = GUIFonts.loadFont("resource/fonts/Roboto.ttf", 15);
        scriptEditorFont = GUIFonts.loadFont("resource/fonts/JetBrainsMono.ttf", 20);
    }
}
