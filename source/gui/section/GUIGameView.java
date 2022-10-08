package gui.section;

import camera.PerspectiveCamera;
import editor.Editor;
import gui.abs.GUISection;
import imgui.ImGui;
import imgui.flag.ImGuiStyleVar;
import imgui.flag.ImGuiWindowFlags;
import math.Float4;
import math.Int2;

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
        if (editor.scene == null || editor.scene.mainCamera == null) {
            editor.gameRenderer.clear(null);
            return;
        }

        editor.gameRenderer.clear(editor.scene.mainCamera);

        editor.window.getContext().setViewport(windowSize);
        editor.gameRenderer.resize(windowSize);

        if (editor.scene.mainCamera.getClass() == PerspectiveCamera.class) {
            ((PerspectiveCamera) editor.scene.mainCamera).updateAspect(windowSize);
        }

        editor.gameRenderer.renderScene(editor.scene, editor.scene.mainCamera);
    }

    private void displayFrame() {
        ImGui.image(editor.gameRenderer.renderBuffer.getColorMap().getBuffer(), windowSize.x, windowSize.y, 0, 1, 1, 0);
    }

    @Override
    public void renderGUI() {
        ImGui.pushStyleVar(ImGuiStyleVar.WindowPadding, 0, 0);

        if (ImGui.begin("Game View", ImGuiWindowFlags.NoScrollbar)) {
            updateWindowInfo();

            renderScene();
            displayFrame();
        }
        ImGui.end();

        ImGui.popStyleVar();
    }
}
