package gui;

import imgui.*;
import imgui.flag.*;
import math.*;

import java.awt.*;

public final class GUIStyle {
    private GUIStyle() {}

    public static Float3 black;
    public static Float3 dark;
    public static Float3 normal;
    public static Float3 light;
    public static Float3 white;
    public static Float3 special;

    static {
        loadDefaults();
    }

    public static void loadDefaults() {
        black = new Float3(new Color(20, 20, 20));
        dark = new Float3(new Color(25, 25, 25));
        normal = new Float3(new Color(50, 50, 50));
        light = new Float3(new Color(75, 75, 75));
        white = new Float3(new Color(242, 242, 242));
        special = new Float3(new Color(218, 122, 50));
        reloadStyle();
    }

    public static void reloadStyle() {
        ImGui.styleColorsDark();
        ImGuiStyle style = ImGui.getStyle();

        style.setWindowPadding(15, 15);
        style.setWindowRounding(2);
        style.setFramePadding(5, 5);
        style.setFrameRounding(2);
        style.setItemSpacing(12, 8);
        style.setItemInnerSpacing(8, 6);
        style.setSelectableTextAlign(0.5f, 0.5f);
        style.setIndentSpacing(25);
        style.setScrollbarSize(15);
        style.setScrollbarRounding(9);
        style.setGrabMinSize(5);
        style.setGrabRounding(3);
        style.setPopupBorderSize(1);
        style.setPopupRounding(5);
        style.setChildBorderSize(1);
        style.setChildRounding(5);

        style.setColor(ImGuiCol.Text, white.x, white.y, white.z, 1);
        style.setColor(ImGuiCol.TextDisabled, light.x, light.y, light.z, 1);
        style.setColor(ImGuiCol.TextSelectedBg, light.x, light.y, light.z, 1);

        style.setColor(ImGuiCol.WindowBg, black.x, black.y, black.z, 1);
        style.setColor(ImGuiCol.ChildBg, dark.x, dark.y, dark.z, 1);
        style.setColor(ImGuiCol.PopupBg, dark.x, dark.y, dark.z, 1);

        style.setColor(ImGuiCol.Border, light.x, light.y, light.z, 1);
        style.setColor(ImGuiCol.BorderShadow, normal.x, normal.y, normal.z, 1);

        style.setColor(ImGuiCol.FrameBg, dark.x, dark.y, dark.z, 1);
        style.setColor(ImGuiCol.FrameBgHovered, normal.x, normal.y, normal.z, 1);
        style.setColor(ImGuiCol.FrameBgActive, light.x, light.y, light.z, 1);

        style.setColor(ImGuiCol.TitleBg, dark.x, dark.y, dark.z, 1);
        style.setColor(ImGuiCol.TitleBgActive, dark.x, dark.y, dark.z, 1);
        style.setColor(ImGuiCol.TitleBgCollapsed, dark.x, dark.y, dark.z, 1);

        style.setColor(ImGuiCol.MenuBarBg, dark.x, dark.y, dark.z, 1);

        style.setColor(ImGuiCol.ScrollbarBg, dark.x, dark.y, dark.z, 1);
        style.setColor(ImGuiCol.ScrollbarGrab, light.x, light.y, light.z, 1);
        style.setColor(ImGuiCol.ScrollbarGrabHovered, normal.x, normal.y, normal.z, 1);
        style.setColor(ImGuiCol.ScrollbarGrabActive, light.x, light.y, light.z, 1);

        style.setColor(ImGuiCol.CheckMark, special.x, special.y, special.z, 1);

        style.setColor(ImGuiCol.SliderGrab, special.x, special.y, special.z, 1);
        style.setColor(ImGuiCol.SliderGrabActive, special.x, special.y, special.z, 1);

        style.setColor(ImGuiCol.Button, normal.x, normal.y, normal.z, 1);
        style.setColor(ImGuiCol.ButtonHovered, light.x, light.y, light.z, 1);
        style.setColor(ImGuiCol.ButtonActive, light.x, light.y, light.z, 1);

        style.setColor(ImGuiCol.Header, normal.x, normal.y, normal.z, 0.1f);
        style.setColor(ImGuiCol.HeaderHovered, light.x, light.y, light.z, 0.25f);
        style.setColor(ImGuiCol.HeaderActive, special.x, special.y, special.z, 1);

        style.setColor(ImGuiCol.Separator, normal.x, normal.y, normal.z, 1);
        style.setColor(ImGuiCol.SeparatorHovered, light.x, light.y, light.z, 1);
        style.setColor(ImGuiCol.SeparatorActive, special.x, special.y, special.z, 1);

        style.setColor(ImGuiCol.ResizeGrip, normal.x, normal.y, normal.z, 1);
        style.setColor(ImGuiCol.ResizeGripHovered, light.x, light.y, light.z, 1);
        style.setColor(ImGuiCol.ResizeGripActive, special.x, special.y, special.z, 1);

        style.setColor(ImGuiCol.Tab, normal.x, normal.y, normal.z, 1);
        style.setColor(ImGuiCol.TabHovered, special.x, special.y, special.z, 1);
        style.setColor(ImGuiCol.TabActive, special.x, special.y, special.z, 1);
        style.setColor(ImGuiCol.TabUnfocused, normal.x, normal.y, normal.z, 1);
        style.setColor(ImGuiCol.TabUnfocusedActive, light.x, light.y, light.z, 1);

        style.setColor(ImGuiCol.DockingPreview, special.x, special.y, special.z, 1);
        style.setColor(ImGuiCol.DockingEmptyBg, normal.x, normal.y, normal.z, 1);

        style.setColor(ImGuiCol.PlotLines, special.x, special.y, special.z, 1);
        style.setColor(ImGuiCol.PlotLinesHovered, light.x, light.y, light.z, 1);
        style.setColor(ImGuiCol.PlotHistogram, special.x, special.y, special.z, 1);
        style.setColor(ImGuiCol.PlotHistogramHovered, light.x, light.y, light.z, 1);

        style.setColor(ImGuiCol.TableHeaderBg, normal.x, normal.y, normal.z, 1);
        style.setColor(ImGuiCol.TableBorderStrong, dark.x, dark.y, dark.z, 1);
        style.setColor(ImGuiCol.TableBorderLight, light.x, light.y, light.z, 1);
        style.setColor(ImGuiCol.TableRowBg, dark.x, dark.y, dark.z, 1);
        style.setColor(ImGuiCol.TableRowBgAlt, normal.x, normal.y, normal.z, 1);

        style.setColor(ImGuiCol.DragDropTarget, special.x, special.y, special.z, 1);

        style.setColor(ImGuiCol.NavHighlight, special.x, special.y, special.z, 1);
        style.setColor(ImGuiCol.NavWindowingHighlight, special.x, special.y, special.z, 1);
        style.setColor(ImGuiCol.NavWindowingDimBg, normal.x, normal.y, normal.z, 1);

        style.setColor(ImGuiCol.ModalWindowDimBg, normal.x, normal.y, normal.z, 1);
    }

    public static void setFont(String filepath, int size) {
        ImFontAtlas fontAtlas = ImGui.getIO().getFonts();

        fontAtlas.clear();
        fontAtlas.addFontFromFileTTF(filepath, size, fontAtlas.getGlyphRangesDefault());

        fontAtlas.setFlags(ImGuiFreeTypeBuilderFlags.LightHinting);
        fontAtlas.build();
    }
}
