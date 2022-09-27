package gui;

import glparts.Disposable;
import imgui.*;
import imgui.flag.*;
import imgui.gl3.*;
import imgui.glfw.*;
import window.*;

import java.util.ArrayList;

public class GUIRenderer extends ArrayList<GUIRenderable> implements Disposable {
    private final ImGuiImplGlfw implGlfw = new ImGuiImplGlfw();
    private final ImGuiImplGl3 implGl3 = new ImGuiImplGl3();

    public GUIRenderer(Window window) {
        ImGui.createContext();
        ImGui.getIO().setConfigFlags(ImGuiConfigFlags.DockingEnable);

        implGlfw.init(window.window, true);
        implGl3.init();

        loadCustomTheme();
    }

    @Override
    public void dispose() {
        implGl3.dispose();
        implGlfw.dispose();
        ImGui.destroyContext();
    }

    public void render() {
        implGlfw.newFrame();
        ImGui.newFrame();

        ImGui.dockSpaceOverViewport(ImGui.getMainViewport(), ImGuiDockNodeFlags.PassthruCentralNode);

        for (GUIRenderable guiRenderable : this) {
            guiRenderable.onGUIRender();
        }

        ImGui.render();
        implGl3.renderDrawData(ImGui.getDrawData());
    }

    private void loadCustomTheme() {
        ImGuiIO io = ImGui.getIO();

        ImGui.styleColorsDark();
        ImGuiStyle style = ImGui.getStyle();

        style.setWindowPadding(15.0f, 15.0f);
        style.setWindowRounding(2.0f);
        style.setFramePadding(5.0f, 5.0f);
        style.setFrameRounding(2.0f);
        style.setItemSpacing(12.0f, 8.0f);
        style.setItemInnerSpacing(8.0f, 6.0f);
        style.setSelectableTextAlign(0.5f, 0.5f);
        style.setIndentSpacing(25.0f);
        style.setScrollbarSize(15.0f);
        style.setScrollbarRounding(9.0f);
        style.setGrabMinSize(5.0f);
        style.setGrabRounding(3.0f);
        style.setPopupBorderSize(1.0f);
        style.setPopupRounding(5.0f);
        style.setChildBorderSize(1.0f);
        style.setChildRounding(5.0f);

	    ImVec4 colorDark = new ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
	    ImVec4 colorMid = new ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
	    ImVec4 colorLight = new ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
	    ImVec4 colorSpec = new ImVec4(0.7f, 0.4f, 0.0f, 1.0f);

        style.setColor(ImGuiCol.Text, 0.95f, 0.95f, 0.95f, 1.0f);
        style.setColor(ImGuiCol.TextDisabled, colorLight.x, colorLight.y, colorLight.z, colorLight.w);
        style.setColor(ImGuiCol.WindowBg, 0.077f, 0.077f, 0.077f, 1.0f);
        style.setColor(ImGuiCol.ChildBg, colorDark.x, colorDark.y, colorDark.z, colorDark.w);
        style.setColor(ImGuiCol.PopupBg, colorDark.x, colorDark.y, colorDark.z, colorDark.w);
        style.setColor(ImGuiCol.Border, colorLight.x, colorLight.y, colorLight.z, colorLight.w);
        style.setColor(ImGuiCol.BorderShadow, colorMid.x, colorMid.y, colorMid.z, colorMid.w);
        style.setColor(ImGuiCol.FrameBg, colorDark.x, colorDark.y, colorDark.z, colorDark.w);
        style.setColor(ImGuiCol.FrameBgHovered, colorMid.x, colorMid.y, colorMid.z, colorMid.w);
        style.setColor(ImGuiCol.FrameBgActive, colorLight.x, colorLight.y, colorLight.z, colorLight.w);
        style.setColor(ImGuiCol.TitleBg, colorDark.x, colorDark.y, colorDark.z, colorDark.w);
        style.setColor(ImGuiCol.TitleBgActive, colorDark.x, colorDark.y, colorDark.z, colorDark.w);
        style.setColor(ImGuiCol.TitleBgCollapsed, colorDark.x, colorDark.y, colorDark.z, colorDark.w);
        style.setColor(ImGuiCol.MenuBarBg, colorDark.x, colorDark.y, colorDark.z, colorDark.w);
        style.setColor(ImGuiCol.ScrollbarBg, colorDark.x, colorDark.y, colorDark.z, colorDark.w);
        style.setColor(ImGuiCol.ScrollbarGrab, colorLight.x, colorLight.y, colorLight.z, colorLight.w);
        style.setColor(ImGuiCol.ScrollbarGrabHovered, colorMid.x, colorMid.y, colorMid.z, colorMid.w);
        style.setColor(ImGuiCol.ScrollbarGrabActive, colorLight.x, colorLight.y, colorLight.z, colorLight.w);
        style.setColor(ImGuiCol.CheckMark, colorSpec.x, colorSpec.y, colorSpec.z, colorSpec.w);
        style.setColor(ImGuiCol.SliderGrab, colorSpec.x, colorSpec.y, colorSpec.z, colorSpec.w);
        style.setColor(ImGuiCol.SliderGrabActive, colorSpec.x, colorSpec.y, colorSpec.z, colorSpec.w);
        style.setColor(ImGuiCol.Button, colorMid.x, colorMid.y, colorMid.z, colorMid.w);
        style.setColor(ImGuiCol.ButtonHovered, colorLight.x, colorLight.y, colorLight.z, colorLight.w);
        style.setColor(ImGuiCol.ButtonActive, colorLight.x, colorLight.y, colorLight.z, colorLight.w);
        style.setColor(ImGuiCol.Header, colorMid.x, colorMid.y, colorMid.z, colorMid.w);
        style.setColor(ImGuiCol.HeaderHovered, colorLight.x, colorLight.y, colorLight.z, colorLight.w);
        style.setColor(ImGuiCol.HeaderActive, colorSpec.x, colorSpec.y, colorSpec.z, colorSpec.w);
        style.setColor(ImGuiCol.Separator, colorMid.x, colorMid.y, colorMid.z, colorMid.w);
        style.setColor(ImGuiCol.SeparatorHovered, colorLight.x, colorLight.y, colorLight.z, colorLight.w);
        style.setColor(ImGuiCol.SeparatorActive, colorSpec.x, colorSpec.y, colorSpec.z, colorSpec.w);
        style.setColor(ImGuiCol.ResizeGrip, colorMid.x, colorMid.y, colorMid.z, colorMid.w);
        style.setColor(ImGuiCol.ResizeGripHovered, colorLight.x, colorLight.y, colorLight.z, colorLight.w);
        style.setColor(ImGuiCol.ResizeGripActive, colorSpec.x, colorSpec.y, colorSpec.z, colorSpec.w);
        style.setColor(ImGuiCol.Tab, colorMid.x, colorMid.y, colorMid.z, colorMid.w);
        style.setColor(ImGuiCol.TabHovered, colorSpec.x, colorSpec.y, colorSpec.z, colorSpec.w);
        style.setColor(ImGuiCol.TabActive, colorSpec.x, colorSpec.y, colorSpec.z, colorSpec.w);
        style.setColor(ImGuiCol.TabUnfocused, colorMid.x, colorMid.y, colorMid.z, colorMid.w);
        style.setColor(ImGuiCol.TabUnfocusedActive, colorLight.x, colorLight.y, colorLight.z, colorLight.w);
        style.setColor(ImGuiCol.DockingPreview, colorSpec.x, colorSpec.y, colorSpec.z, colorSpec.w);
        style.setColor(ImGuiCol.DockingEmptyBg, colorMid.x, colorMid.y, colorMid.z, colorMid.w);
        style.setColor(ImGuiCol.PlotLines, colorSpec.x, colorSpec.y, colorSpec.z, colorSpec.w);
        style.setColor(ImGuiCol.PlotLinesHovered, colorLight.x, colorLight.y, colorLight.z, colorLight.w);
        style.setColor(ImGuiCol.PlotHistogram, colorSpec.x, colorSpec.y, colorSpec.z, colorSpec.w);
        style.setColor(ImGuiCol.PlotHistogramHovered, colorLight.x, colorLight.y, colorLight.z, colorLight.w);
        style.setColor(ImGuiCol.TableHeaderBg, colorMid.x, colorMid.y, colorMid.z, colorMid.w);
        style.setColor(ImGuiCol.TableBorderStrong, colorDark.x, colorDark.y, colorDark.z, colorDark.w);
        style.setColor(ImGuiCol.TableBorderLight, colorLight.x, colorLight.y, colorLight.z, colorLight.w);
        style.setColor(ImGuiCol.TableRowBg, colorDark.x, colorDark.y, colorDark.z, colorDark.w);
        style.setColor(ImGuiCol.TableRowBgAlt, colorMid.x, colorMid.y, colorMid.z, colorMid.w);
        style.setColor(ImGuiCol.TextSelectedBg, colorLight.x, colorLight.y, colorLight.z, colorLight.w);
        style.setColor(ImGuiCol.DragDropTarget, colorSpec.x, colorSpec.y, colorSpec.z, colorSpec.w);
        style.setColor(ImGuiCol.NavHighlight, colorSpec.x, colorSpec.y, colorSpec.z, colorSpec.w);
        style.setColor(ImGuiCol.NavWindowingHighlight, colorSpec.x, colorSpec.y, colorSpec.z, colorSpec.w);
        style.setColor(ImGuiCol.NavWindowingDimBg, colorMid.x, colorMid.y, colorMid.z, colorMid.w);
        style.setColor(ImGuiCol.ModalWindowDimBg, colorMid.x, colorMid.y, colorMid.z, colorMid.w);
    }
}
