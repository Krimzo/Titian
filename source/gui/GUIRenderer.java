package gui;

import callback.EmptyCallback;
import glparts.Texture;
import gui.abs.GUIRenderable;
import imgui.extension.imguizmo.ImGuizmo;
import glparts.abs.Disposable;
import imgui.*;
import imgui.flag.*;
import imgui.gl3.*;
import imgui.glfw.*;
import math.Float4;
import window.*;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

public class GUIRenderer extends ArrayList<GUIRenderable> implements Disposable {
    private final ImGuiImplGlfw implGlfw = new ImGuiImplGlfw();
    private final ImGuiImplGl3 implGl3 = new ImGuiImplGl3();

    public final Map<String, Texture> predefineTextures = new HashMap<>();

    public GUIRenderer(Window window) {
        ImGui.createContext();
        ImGui.getIO().setConfigFlags(ImGuiConfigFlags.DockingEnable);

        implGlfw.init(window.getWindow(), true);
        implGl3.init();

        loadCustomTheme();

        // Viewport
        predefineTextures.put("PlayIcon", new Texture(null, null, window.getContext(), "resource/textures/control/play.png", false));
        predefineTextures.put("StopIcon", new Texture(null, null, window.getContext(), "resource/textures/control/stop.png", false));
        predefineTextures.put("WireIcon", new Texture(null, null, window.getContext(), "resource/textures/control/wire.png", false));
        predefineTextures.put("SolidIcon", new Texture(null, null, window.getContext(), "resource/textures/control/solid.png", false));

        // Explorer
        predefineTextures.put("FolderIcon", new Texture(null, null, window.getContext(), "resource/textures/explorer/folder.png", false));
        predefineTextures.put("EmptyFolderIcon", new Texture(null, null, window.getContext(), "resource/textures/explorer/folder_empty.png", false));
        predefineTextures.put("FileIcon", new Texture(null, null, window.getContext(), "resource/textures/explorer/file.png", false));
        predefineTextures.put("ImageFileIcon", new Texture(null, null, window.getContext(), "resource/textures/explorer/image.png", false));
        predefineTextures.put("MeshFileIcon", new Texture(null, null, window.getContext(), "resource/textures/explorer/mesh.png", false));
        predefineTextures.put("ScriptFileIcon", new Texture(null, null, window.getContext(), "resource/textures/explorer/script.png", false));
        predefineTextures.put("CodeFileIcon", new Texture(null, null, window.getContext(), "resource/textures/explorer/code.png", false));
    }

    @Override
    public void dispose() {
        for (var obj : predefineTextures.entrySet()) {
            obj.getValue().dispose();
        }
        predefineTextures.clear();

        implGl3.dispose();
        implGlfw.dispose();
        ImGui.destroyContext();
    }

    public void render() {
        implGlfw.newFrame();
        ImGui.newFrame();
        ImGuizmo.beginFrame();

        ImGui.dockSpaceOverViewport(ImGui.getMainViewport(), ImGuiDockNodeFlags.PassthruCentralNode);

        for (GUIRenderable guiRenderable : this) {
            guiRenderable.renderGUI();
        }

        ImGui.render();
        implGl3.renderDrawData(ImGui.getDrawData());
    }

    public static void useColor(int styleType, Float4 color, EmptyCallback callback) {
        ImGui.pushStyleColor(styleType, color.x, color.y, color.z, color.w);
        callback.method();
        ImGui.popStyleColor();
    }

    public static void loadCustomTheme() {
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

	    Float4 colorDark = new Float4(0.1f, 0.1f, 0.1f, 1.0f);
	    Float4 colorNormal = new Float4(0.2f, 0.2f, 0.2f, 1.0f);
	    Float4 colorLight = new Float4(0.3f, 0.3f, 0.3f, 1.0f);
	    Float4 colorSpecial = new Float4(0.7f, 0.4f, 0.0f, 1.0f);

        style.setColor(ImGuiCol.Text, 0.95f, 0.95f, 0.95f, 1.0f);
        style.setColor(ImGuiCol.TextDisabled, colorLight.x, colorLight.y, colorLight.z, colorLight.w);
        style.setColor(ImGuiCol.WindowBg, 0.077f, 0.077f, 0.077f, 1.0f);
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
