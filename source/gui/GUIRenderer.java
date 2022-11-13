package gui;

import gui.abs.GUIRenderable;
import gui.helper.GUIFonts;
import gui.helper.GUIStyle;
import imgui.ImGui;
import imgui.extension.imguizmo.ImGuizmo;
import imgui.flag.ImGuiConfigFlags;
import imgui.flag.ImGuiDockNodeFlags;
import imgui.gl3.ImGuiImplGl3;
import imgui.glfw.ImGuiImplGlfw;
import utility.nncollection.NNArrayList;
import window.Window;

public class GUIRenderer extends NNArrayList<GUIRenderable> {
    private final ImGuiImplGlfw implGlfw = new ImGuiImplGlfw();
    private final ImGuiImplGl3 implGl3 = new ImGuiImplGl3();

    public final GUIFontData fonts;
    public final GUITextureData textures;

    public GUIRenderer(Window window) {
        ImGui.createContext();
        ImGui.getIO().setConfigFlags(ImGuiConfigFlags.DockingEnable);

        GUIStyle.reloadStyle();
        GUIFonts.clearGlobalAtlas();
        fonts = new GUIFontData();

        implGlfw.init(window.getWindow(), true);
        implGl3.init("#version 330");

        textures = new GUITextureData(window.getContext());
    }

    public void destroy() {
        implGl3.dispose();
        implGlfw.dispose();
        ImGui.destroyContext();
    }

    public void render() {
        implGlfw.newFrame();
        ImGui.newFrame();
        ImGuizmo.beginFrame();

        ImGui.dockSpaceOverViewport(ImGui.getMainViewport(), ImGuiDockNodeFlags.PassthruCentralNode);

        for (GUIRenderable renderable : this) {
            renderable.renderGUI();
        }

        ImGui.render();
        implGl3.renderDrawData(ImGui.getDrawData());
    }
}
