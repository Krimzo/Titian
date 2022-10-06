package gui;

import callback.EmptyCallback;
import imgui.ImGui;
import imgui.ImGuiStyle;
import imgui.flag.ImGuiCol;
import imgui.internal.ImGuiContext;
import math.Float4;

public final class GUI {
    private GUI() {}

    public static void useColor(int styleType, Float4 color, EmptyCallback callback) {
        ImGui.pushStyleColor(styleType, color.x, color.y, color.z, color.w);
        callback.method();
        ImGui.popStyleColor();
    }

    public static void loadCustomTheme() {
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

        Float4 colorDark = new Float4(0.1f, 0.1f, 0.1f, 1);
        Float4 colorNormal = new Float4(0.2f, 0.2f, 0.2f, 1);
        Float4 colorLight = new Float4(0.3f, 0.3f, 0.3f, 1);
        Float4 colorSpecial = new Float4(0.7f, 0.4f, 0, 1);

        style.setColor(ImGuiCol.Text, 0.95f, 0.95f, 0.95f, 1);
        style.setColor(ImGuiCol.TextDisabled, colorLight.x, colorLight.y, colorLight.z, colorLight.w);
        style.setColor(ImGuiCol.WindowBg, 0.077f, 0.077f, 0.077f, 1);
        style.setColor(ImGuiCol.ChildBg, colorDark.x, colorDark.y, colorDark.z, colorDark.w);
        style.setColor(ImGuiCol.PopupBg, colorDark.x, colorDark.y, colorDark.z, colorDark.w);
        style.setColor(ImGuiCol.Border, colorLight.x, colorLight.y, colorLight.z, colorLight.w);
        style.setColor(ImGuiCol.BorderShadow, colorNormal.x, colorNormal.y, colorNormal.z, colorNormal.w);
        style.setColor(ImGuiCol.FrameBg, colorDark.x, colorDark.y, colorDark.z, colorDark.w);
        style.setColor(ImGuiCol.FrameBgHovered, colorNormal.x, colorNormal.y, colorNormal.z, colorNormal.w);
        style.setColor(ImGuiCol.FrameBgActive, colorLight.x, colorLight.y, colorLight.z, colorLight.w);
        style.setColor(ImGuiCol.TitleBg, colorDark.x, colorDark.y, colorDark.z, colorDark.w);
        style.setColor(ImGuiCol.TitleBgActive, colorDark.x, colorDark.y, colorDark.z, colorDark.w);
        style.setColor(ImGuiCol.TitleBgCollapsed, colorDark.x, colorDark.y, colorDark.z, colorDark.w);
        style.setColor(ImGuiCol.MenuBarBg, colorDark.x, colorDark.y, colorDark.z, colorDark.w);
        style.setColor(ImGuiCol.ScrollbarBg, colorDark.x, colorDark.y, colorDark.z, colorDark.w);
        style.setColor(ImGuiCol.ScrollbarGrab, colorLight.x, colorLight.y, colorLight.z, colorLight.w);
        style.setColor(ImGuiCol.ScrollbarGrabHovered, colorNormal.x, colorNormal.y, colorNormal.z, colorNormal.w);
        style.setColor(ImGuiCol.ScrollbarGrabActive, colorLight.x, colorLight.y, colorLight.z, colorLight.w);
        style.setColor(ImGuiCol.CheckMark, colorSpecial.x, colorSpecial.y, colorSpecial.z, colorSpecial.w);
        style.setColor(ImGuiCol.SliderGrab, colorSpecial.x, colorSpecial.y, colorSpecial.z, colorSpecial.w);
        style.setColor(ImGuiCol.SliderGrabActive, colorSpecial.x, colorSpecial.y, colorSpecial.z, colorSpecial.w);
        style.setColor(ImGuiCol.Button, colorNormal.x, colorNormal.y, colorNormal.z, colorNormal.w);
        style.setColor(ImGuiCol.ButtonHovered, colorLight.x, colorLight.y, colorLight.z, colorLight.w);
        style.setColor(ImGuiCol.ButtonActive, colorLight.x, colorLight.y, colorLight.z, colorLight.w);
        style.setColor(ImGuiCol.Header, colorNormal.x, colorNormal.y, colorNormal.z, colorNormal.w);
        style.setColor(ImGuiCol.HeaderHovered, colorLight.x, colorLight.y, colorLight.z, colorLight.w);
        style.setColor(ImGuiCol.HeaderActive, colorSpecial.x, colorSpecial.y, colorSpecial.z, colorSpecial.w);
        style.setColor(ImGuiCol.Separator, colorNormal.x, colorNormal.y, colorNormal.z, colorNormal.w);
        style.setColor(ImGuiCol.SeparatorHovered, colorLight.x, colorLight.y, colorLight.z, colorLight.w);
        style.setColor(ImGuiCol.SeparatorActive, colorSpecial.x, colorSpecial.y, colorSpecial.z, colorSpecial.w);
        style.setColor(ImGuiCol.ResizeGrip, colorNormal.x, colorNormal.y, colorNormal.z, colorNormal.w);
        style.setColor(ImGuiCol.ResizeGripHovered, colorLight.x, colorLight.y, colorLight.z, colorLight.w);
        style.setColor(ImGuiCol.ResizeGripActive, colorSpecial.x, colorSpecial.y, colorSpecial.z, colorSpecial.w);
        style.setColor(ImGuiCol.Tab, colorNormal.x, colorNormal.y, colorNormal.z, colorNormal.w);
        style.setColor(ImGuiCol.TabHovered, colorSpecial.x, colorSpecial.y, colorSpecial.z, colorSpecial.w);
        style.setColor(ImGuiCol.TabActive, colorSpecial.x, colorSpecial.y, colorSpecial.z, colorSpecial.w);
        style.setColor(ImGuiCol.TabUnfocused, colorNormal.x, colorNormal.y, colorNormal.z, colorNormal.w);
        style.setColor(ImGuiCol.TabUnfocusedActive, colorLight.x, colorLight.y, colorLight.z, colorLight.w);
        style.setColor(ImGuiCol.DockingPreview, colorSpecial.x, colorSpecial.y, colorSpecial.z, colorSpecial.w);
        style.setColor(ImGuiCol.DockingEmptyBg, colorNormal.x, colorNormal.y, colorNormal.z, colorNormal.w);
        style.setColor(ImGuiCol.PlotLines, colorSpecial.x, colorSpecial.y, colorSpecial.z, colorSpecial.w);
        style.setColor(ImGuiCol.PlotLinesHovered, colorLight.x, colorLight.y, colorLight.z, colorLight.w);
        style.setColor(ImGuiCol.PlotHistogram, colorSpecial.x, colorSpecial.y, colorSpecial.z, colorSpecial.w);
        style.setColor(ImGuiCol.PlotHistogramHovered, colorLight.x, colorLight.y, colorLight.z, colorLight.w);
        style.setColor(ImGuiCol.TableHeaderBg, colorNormal.x, colorNormal.y, colorNormal.z, colorNormal.w);
        style.setColor(ImGuiCol.TableBorderStrong, colorDark.x, colorDark.y, colorDark.z, colorDark.w);
        style.setColor(ImGuiCol.TableBorderLight, colorLight.x, colorLight.y, colorLight.z, colorLight.w);
        style.setColor(ImGuiCol.TableRowBg, colorDark.x, colorDark.y, colorDark.z, colorDark.w);
        style.setColor(ImGuiCol.TableRowBgAlt, colorNormal.x, colorNormal.y, colorNormal.z, colorNormal.w);
        style.setColor(ImGuiCol.TextSelectedBg, colorLight.x, colorLight.y, colorLight.z, colorLight.w);
        style.setColor(ImGuiCol.DragDropTarget, colorSpecial.x, colorSpecial.y, colorSpecial.z, colorSpecial.w);
        style.setColor(ImGuiCol.NavHighlight, colorSpecial.x, colorSpecial.y, colorSpecial.z, colorSpecial.w);
        style.setColor(ImGuiCol.NavWindowingHighlight, colorSpecial.x, colorSpecial.y, colorSpecial.z, colorSpecial.w);
        style.setColor(ImGuiCol.NavWindowingDimBg, colorNormal.x, colorNormal.y, colorNormal.z, colorNormal.w);
        style.setColor(ImGuiCol.ModalWindowDimBg, colorNormal.x, colorNormal.y, colorNormal.z, colorNormal.w);
    }
}
