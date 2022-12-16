package gui

import gui.helper.GUIFonts
import imgui.ImFont

class GUIFontData {
    val defaultFont: ImFont?
    val scriptEditorFont: ImFont?

    init {
        defaultFont = GUIFonts.loadFont("resource/fonts/Roboto.ttf", 15)
        scriptEditorFont = GUIFonts.loadFont("resource/fonts/JetBrainsMono.ttf", 20)
    }
}