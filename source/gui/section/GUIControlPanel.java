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

    @Override
    public void dispose() {

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
            final int padding = 5;
            ImVec2 available = ImGui.getContentRegionAvail();
            float buttonSize = Math.min(available.x, available.y) - padding * 2;

            Texture playStateTexture = editor.data.gameRunning ? editor.guiRenderer.textures.stopIcon : editor.guiRenderer.textures.playIcon;
            if (ImGui.imageButton(playStateTexture.getBuffer(), buttonSize, buttonSize, 0, 0, 1, 1, padding)) {
                if (!editor.data.gameRunning) {
                    editor.data.gameRunning = true;
                    onGameStart();
                }
                else {
                    editor.data.gameRunning = false;
                    onGameEnd();
                }
            }

            ImGui.sameLine();
            ImGui.setCursorPosX(ImGui.getWindowContentRegionMaxX() - buttonSize - padding * 2);

            Texture wireframeTexture = editor.data.wireframeState ? editor.guiRenderer.textures.solidIcon : editor.guiRenderer.textures.wireIcon;
            if (ImGui.imageButton(wireframeTexture.getBuffer(), buttonSize, buttonSize, 0, 0, 1, 1, padding)) {
                editor.data.wireframeState = !editor.data.wireframeState;
            }
        }
        ImGui.end();
    }
}
