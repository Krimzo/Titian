package gui

import gui.helper.GUIFonts
import imgui.ImFont

class GUIFontData {
    val defaultFont: ImFont = GUIFonts.loadFont("resource/fonts/Roboto.ttf", 15)
    val scriptEditorFont: ImFont = GUIFonts.loadFont("resource/fonts/JetBrainsMono.ttf", 20)
}
