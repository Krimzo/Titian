package gui;

import glparts.Texture;
import gui.abs.GUIRenderable;
import imgui.extension.imguizmo.ImGuizmo;
import glparts.abs.Disposable;
import imgui.*;
import imgui.flag.*;
import imgui.gl3.*;
import imgui.glfw.*;
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

        GUIStyle.reloadStyle();
        GUIStyle.setFont("resource/fonts/Roboto.ttf", 15);

        implGlfw.init(window.getWindow(), true);
        implGl3.init("#version 330");

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
}
