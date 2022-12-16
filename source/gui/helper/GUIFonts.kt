package gui.helper

import imgui.ImFont
import imgui.ImFontAtlas
import imgui.ImGui
import imgui.flag.ImGuiFreeTypeBuilderFlags

object GUIFonts {
    fun loadFont(filepath: String?, size: Int): ImFont {
        val fontAtlas = globalAtlas
        val font = fontAtlas.addFontFromFileTTF(filepath, size.toFloat(), fontAtlas.glyphRangesDefault)
        fontAtlas.flags = ImGuiFreeTypeBuilderFlags.LightHinting
        fontAtlas.build()
        return font
    }

    val globalAtlas: ImFontAtlas
        get() = ImGui.getIO().fonts

    fun clearGlobalAtlas() {
        globalAtlas.clear()
    }

    fun buildGlobalAtlas() {
        globalAtlas.build()
    }
}