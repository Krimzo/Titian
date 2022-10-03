package gui.section;

import editor.Editor;
import glparts.Texture;
import gui.abs.GUISection;
import imgui.ImGui;
import imgui.ImVec2;
import imgui.flag.ImGuiWindowFlags;
import scene.Scene;

public final class GUIControlPanel extends GUISection {
    private static final String SAVED_RUN_SCENE = "resource/scenes/saved_run.scene";

    public GUIControlPanel(Editor editor) {
        super(editor);
    }

    @Override
    public void renderGUI() {
        if (ImGui.begin("Control Panel", ImGuiWindowFlags.NoScrollbar)) {
            ImVec2 windowRegion = ImGui.getWindowContentRegionMax();

            float buttonSize = windowRegion.y * 0.5f;

            boolean gameRunning = (boolean) editor.savedData.get("GameRunning");
            Texture playStateTexture = gameRunning ? editor.guiRenderer.predefineTextures.get("StopIcon") : editor.guiRenderer.predefineTextures.get("PlayIcon");
            if (ImGui.imageButton(playStateTexture.getBuffer(), buttonSize, buttonSize)) {
                if (!gameRunning) {
                    editor.scene.toFile(SAVED_RUN_SCENE);
                }
                else {
                    editor.destroyCurrentScene();
                    editor.scene = Scene.fromFile(SAVED_RUN_SCENE);
                }
                editor.savedData.put("GameRunning", !gameRunning);
            }
            ImGui.sameLine();

            boolean wireframeEnabled = (boolean) editor.savedData.get("WireframeState");
            Texture wireframeTexture = wireframeEnabled ? editor.guiRenderer.predefineTextures.get("SolidIcon") : editor.guiRenderer.predefineTextures.get("WireIcon");
            if (ImGui.imageButton(wireframeTexture.getBuffer(), buttonSize, buttonSize)) {
                editor.savedData.put("WireframeState", !wireframeEnabled);
            }
            ImGui.sameLine();
        }
        ImGui.end();
    }
}
