package gui.helper;

import imgui.ImFont;
import imgui.ImFontAtlas;
import imgui.ImGui;
import imgui.flag.ImGuiFreeTypeBuilderFlags;

public final class GUIFonts {
    private GUIFonts() {}

    public static ImFont loadFont(String filepath, int size) {
        ImFontAtlas fontAtlas = getGlobalAtlas();
        ImFont font = fontAtlas.addFontFromFileTTF(filepath, size, fontAtlas.getGlyphRangesDefault());
        fontAtlas.setFlags(ImGuiFreeTypeBuilderFlags.LightHinting);
        fontAtlas.build();
        return font;
    }

    public static ImFontAtlas getGlobalAtlas() {
        return ImGui.getIO().getFonts();
    }

    public static void clearGlobalAtlas() {
        getGlobalAtlas().clear();
    }

    public static void buildGlobalAtlas() {
        getGlobalAtlas().build();
    }
}
