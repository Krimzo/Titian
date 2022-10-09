package gui.section;

import editor.Editor;
import glparts.Texture;
import gui.abs.GUISection;
import imgui.ImGui;
import imgui.ImVec2;
import imgui.flag.ImGuiWindowFlags;
import scene.Scene;

public final class GUIControlPanel extends GUISection {
    private static final String SAVED_PLAY_PATH = "./temp/play.titian";

    public GUIControlPanel(Editor editor) {
        super(editor);
    }

    private void onGameStart() {
        editor.scene.toFile(SAVED_PLAY_PATH);
        editor.scripter.callStarts(editor.scene);
        editor.timer.reset();
    }

    private void onGameEnd() {
        editor.disposeCurrentScene();
        editor.scene = Scene.fromFile(SAVED_PLAY_PATH, editor);
    }

    @Override
    public void renderGUI() {
        if (ImGui.begin("Control Panel", ImGuiWindowFlags.NoScrollbar)) {
            ImVec2 regionMax = ImGui.getWindowContentRegionMax();
            final float buttonSize = regionMax.y * 0.5f;

            Texture playStateTexture = editor.data.gameRunning ? editor.guiRenderer.predefineTextures.get("StopIcon") : editor.guiRenderer.predefineTextures.get("PlayIcon");
            if (ImGui.imageButton(playStateTexture.getBuffer(), buttonSize, buttonSize)) {
                if (!editor.data.gameRunning) {
                    onGameStart();
                    editor.data.gameRunning = true;
                }
                else {
                    onGameEnd();
                    editor.data.gameRunning = false;
                }
            }

            ImGui.sameLine();

            Texture wireframeTexture = editor.data.wireframeState ? editor.guiRenderer.predefineTextures.get("SolidIcon") : editor.guiRenderer.predefineTextures.get("WireIcon");
            if (ImGui.imageButton(wireframeTexture.getBuffer(), buttonSize, buttonSize)) {
                editor.data.wireframeState = !editor.data.wireframeState;
            }
        }
        ImGui.end();
    }
}
