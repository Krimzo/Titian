package gui.section;

import editor.Editor;
import glparts.Texture;
import gui.abs.GUISection;
import imgui.ImGui;
import imgui.ImVec2;
import imgui.flag.ImGuiWindowFlags;
import scene.Scene;

public final class GUIControlPanel extends GUISection {
    private static final String SAVED_RUN_SCENE = "saved_run.scene";

    public GUIControlPanel(Editor editor) {
        super(editor);
    }

    private void onGameStart() {
        editor.scene.toFile(SAVED_RUN_SCENE);
        editor.scripter.callStarts(editor.scene);
        editor.timer.reset();
    }

    private void onGameEnd() {
        editor.disposeCurrentScene();
        editor.scene = Scene.fromFile(SAVED_RUN_SCENE);
    }

    @Override
    public void renderGUI() {
        if (ImGui.begin("Control Panel", ImGuiWindowFlags.NoScrollbar)) {
            ImVec2 windowRegion = ImGui.getWindowContentRegionMax();

            float buttonSize = windowRegion.y * 0.5f;

            Texture playStateTexture = editor.gameRunning ? editor.guiRenderer.predefineTextures.get("StopIcon") : editor.guiRenderer.predefineTextures.get("PlayIcon");
            if (ImGui.imageButton(playStateTexture.getBuffer(), buttonSize, buttonSize)) {
                if (!editor.gameRunning) {
                    onGameStart();
                    editor.gameRunning = true;
                }
                else {
                    onGameEnd();
                    editor.gameRunning = false;
                }
            }
            ImGui.sameLine();

            Texture wireframeTexture = editor.wireframeState ? editor.guiRenderer.predefineTextures.get("SolidIcon") : editor.guiRenderer.predefineTextures.get("WireIcon");
            if (ImGui.imageButton(wireframeTexture.getBuffer(), buttonSize, buttonSize)) {
                editor.wireframeState = !editor.wireframeState;
            }
            ImGui.sameLine();
        }
        ImGui.end();
    }
}
