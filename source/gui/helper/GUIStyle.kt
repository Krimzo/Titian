package gui.helper

import imgui.ImGui
import imgui.flag.ImGuiCol
import math.Float3
import java.awt.Color

object GUIStyle {
    var black: Float3 = Float3()
    var dark: Float3 = Float3()
    var normal: Float3 = Float3()
    var light: Float3 = Float3()
    var white: Float3 = Float3()
    var special: Float3 = Float3()

    init {
        loadDefaults()
    }

    fun loadDefaults() {
        black = Float3(Color(20, 20, 20))
        dark = Float3(Color(25, 25, 25))
        normal = Float3(Color(50, 50, 50))
        light = Float3(Color(75, 75, 75))
        white = Float3(Color(242, 242, 242))
        special = Float3(Color(218, 122, 50))
        reloadStyle()
    }

    fun reloadStyle() {
        ImGui.styleColorsDark()
        val style = ImGui.getStyle()

        style.windowRounding = 2f
        style.frameRounding = 2f
        style.childBorderSize = 1f
        style.childRounding = 5f
        style.indentSpacing = 25f
        style.scrollbarSize = 15f
        style.scrollbarRounding = 9f
        style.grabMinSize = 5f
        style.grabRounding = 3f
        style.popupBorderSize = 1f
        style.popupRounding = 5f

        style.setWindowPadding(15f, 15f)
        style.setFramePadding(5f, 5f)
        style.setItemSpacing(12f, 8f)
        style.setItemInnerSpacing(8f, 6f)
        style.setSelectableTextAlign(0.5f, 0.5f)

        style.setColor(ImGuiCol.Text, white.x, white.y, white.z, 1f)
        style.setColor(ImGuiCol.TextDisabled, light.x, light.y, light.z, 1f)
        style.setColor(ImGuiCol.TextSelectedBg, light.x, light.y, light.z, 1f)
        style.setColor(ImGuiCol.WindowBg, black.x, black.y, black.z, 1f)
        style.setColor(ImGuiCol.ChildBg, dark.x, dark.y, dark.z, 1f)
        style.setColor(ImGuiCol.PopupBg, dark.x, dark.y, dark.z, 1f)
        style.setColor(ImGuiCol.Border, light.x, light.y, light.z, 1f)
        style.setColor(ImGuiCol.BorderShadow, normal.x, normal.y, normal.z, 1f)
        style.setColor(ImGuiCol.FrameBg, dark.x, dark.y, dark.z, 1f)
        style.setColor(ImGuiCol.FrameBgHovered, normal.x, normal.y, normal.z, 1f)
        style.setColor(ImGuiCol.FrameBgActive, light.x, light.y, light.z, 1f)
        style.setColor(ImGuiCol.TitleBg, dark.x, dark.y, dark.z, 1f)
        style.setColor(ImGuiCol.TitleBgActive, dark.x, dark.y, dark.z, 1f)
        style.setColor(ImGuiCol.TitleBgCollapsed, dark.x, dark.y, dark.z, 1f)
        style.setColor(ImGuiCol.MenuBarBg, dark.x, dark.y, dark.z, 1f)
        style.setColor(ImGuiCol.ScrollbarBg, dark.x, dark.y, dark.z, 1f)
        style.setColor(ImGuiCol.ScrollbarGrab, light.x, light.y, light.z, 1f)
        style.setColor(ImGuiCol.ScrollbarGrabHovered, normal.x, normal.y, normal.z, 1f)
        style.setColor(ImGuiCol.ScrollbarGrabActive, light.x, light.y, light.z, 1f)
        style.setColor(ImGuiCol.CheckMark, special.x, special.y, special.z, 1f)
        style.setColor(ImGuiCol.SliderGrab, special.x, special.y, special.z, 1f)
        style.setColor(ImGuiCol.SliderGrabActive, special.x, special.y, special.z, 1f)
        style.setColor(ImGuiCol.Button, normal.x, normal.y, normal.z, 1f)
        style.setColor(ImGuiCol.ButtonHovered, light.x, light.y, light.z, 1f)
        style.setColor(ImGuiCol.ButtonActive, light.x, light.y, light.z, 1f)
        style.setColor(ImGuiCol.Header, normal.x, normal.y, normal.z, 0.1f)
        style.setColor(ImGuiCol.HeaderHovered, light.x, light.y, light.z, 0.25f)
        style.setColor(ImGuiCol.HeaderActive, special.x, special.y, special.z, 1f)
        style.setColor(ImGuiCol.Separator, light.x, light.y, light.z, 1f)
        style.setColor(ImGuiCol.SeparatorHovered, light.x, light.y, light.z, 1f)
        style.setColor(ImGuiCol.SeparatorActive, special.x, special.y, special.z, 1f)
        style.setColor(ImGuiCol.ResizeGrip, normal.x, normal.y, normal.z, 1f)
        style.setColor(ImGuiCol.ResizeGripHovered, light.x, light.y, light.z, 1f)
        style.setColor(ImGuiCol.ResizeGripActive, special.x, special.y, special.z, 1f)
        style.setColor(ImGuiCol.Tab, normal.x, normal.y, normal.z, 1f)
        style.setColor(ImGuiCol.TabHovered, special.x, special.y, special.z, 1f)
        style.setColor(ImGuiCol.TabActive, special.x, special.y, special.z, 1f)
        style.setColor(ImGuiCol.TabUnfocused, normal.x, normal.y, normal.z, 1f)
        style.setColor(ImGuiCol.TabUnfocusedActive, light.x, light.y, light.z, 1f)
        style.setColor(ImGuiCol.DockingPreview, special.x, special.y, special.z, 1f)
        style.setColor(ImGuiCol.DockingEmptyBg, normal.x, normal.y, normal.z, 1f)
        style.setColor(ImGuiCol.PlotLines, special.x, special.y, special.z, 1f)
        style.setColor(ImGuiCol.PlotLinesHovered, light.x, light.y, light.z, 1f)
        style.setColor(ImGuiCol.PlotHistogram, special.x, special.y, special.z, 1f)
        style.setColor(ImGuiCol.PlotHistogramHovered, light.x, light.y, light.z, 1f)
        style.setColor(ImGuiCol.TableHeaderBg, normal.x, normal.y, normal.z, 1f)
        style.setColor(ImGuiCol.TableBorderStrong, dark.x, dark.y, dark.z, 1f)
        style.setColor(ImGuiCol.TableBorderLight, light.x, light.y, light.z, 1f)
        style.setColor(ImGuiCol.TableRowBg, dark.x, dark.y, dark.z, 1f)
        style.setColor(ImGuiCol.TableRowBgAlt, normal.x, normal.y, normal.z, 1f)
        style.setColor(ImGuiCol.DragDropTarget, special.x, special.y, special.z, 1f)
        style.setColor(ImGuiCol.NavHighlight, special.x, special.y, special.z, 1f)
        style.setColor(ImGuiCol.NavWindowingHighlight, special.x, special.y, special.z, 1f)
        style.setColor(ImGuiCol.NavWindowingDimBg, normal.x, normal.y, normal.z, 1f)
        style.setColor(ImGuiCol.ModalWindowDimBg, normal.x, normal.y, normal.z, 1f)
    }
}