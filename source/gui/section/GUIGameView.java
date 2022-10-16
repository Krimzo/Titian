package gui.section;

import camera.PerspectiveCamera;
import editor.Editor;
import gui.abs.GUISection;
import gui.helper.GUIDisplay;
import imgui.ImGui;
import imgui.flag.ImGuiStyleVar;
import imgui.flag.ImGuiWindowFlags;
import math.Int2;
import utility.Instance;

public final class GUIGameView extends GUISection {
    private Int2 windowSize = new Int2();

    public GUIGameView(Editor editor) {
        super(editor);
    }

    private void updateWindowInfo() {
        int tabSize = (int) ImGui.getWindowContentRegionMinY();
        windowSize = new Int2((int) ImGui.getWindowWidth(), (int) ImGui.getWindowHeight() - tabSize);
    }

    private void renderScene() {
        if (Instance.isNull(editor.getScene().selected.camera)) {
            editor.gameRenderer.clear(null);
            return;
        }

        editor.gameRenderer.clear(editor.getScene().selected.camera);

        editor.window.getContext().setViewport(windowSize);
        editor.gameRenderer.resize(windowSize);

        if (editor.getScene().selected.camera.getClass() == PerspectiveCamera.class) {
            ((PerspectiveCamera) editor.getScene().selected.camera).updateAspect(windowSize);
        }

        editor.gameRenderer.renderScene(editor.getScene(), editor.getScene().selected.camera);
    }

    @Override
    public void renderGUI() {
        ImGui.pushStyleVar(ImGuiStyleVar.WindowPadding, 0, 0);

        if (ImGui.begin("Game View", ImGuiWindowFlags.NoScrollbar)) {
            updateWindowInfo();
            renderScene();
            GUIDisplay.texture(editor.gameRenderer.renderBuffer.getColorMap(), windowSize);
        }
        ImGui.end();

        ImGui.popStyleVar();
    }
}
