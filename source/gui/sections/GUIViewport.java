package gui.sections;

import editor.*;
import entity.Entity;
import entity.components.TransformComponent;
import gui.*;
import imgui.*;
import imgui.extension.imguizmo.ImGuizmo;
import imgui.extension.imguizmo.flag.Mode;
import imgui.extension.imguizmo.flag.Operation;
import imgui.flag.*;
import math.*;
import utility.Pair;

public final class GUIViewport extends GUISection {
    public GUIViewport(Editor editor) {
        super(editor);
    }

    private Pair<Int2> saveViewportInfo() {
        int tabSize = (int) ImGui.getWindowContentRegionMinY();
        Int2 position = new Int2((int) ImGui.getWindowPosX(), (int) ImGui.getWindowPosY() + tabSize);
        Int2 size = new Int2((int) ImGui.getWindowWidth(), (int) ImGui.getWindowHeight() - tabSize);

        editor.savedData.put("ViewportPosition", position);
        editor.savedData.put("ViewportSize", size);

        return new Pair<>(position, size);
    }

    private void objectSelection(Int2 viewportPosition, Int2 viewportSize) {
        if (editor.scene == null || ImGuizmo.isOver()) {
            return;
        }

        Float4 viewportRect = new Float4(
            viewportPosition.x, viewportPosition.y, viewportPosition.x + viewportSize.x, viewportPosition.y + viewportSize.y
        );

        if (ImGui.isMouseHoveringRect(viewportRect.x, viewportRect.y, viewportRect.z, viewportRect.w) && ImGui.isMouseClicked(ImGuiMouseButton.Left)) {
            Float2 mousePosition = new Float2(ImGui.getMousePosX() - viewportPosition.x, ImGui.getMousePosY() - viewportPosition.y);

            int objectIndex = (int) editor.renderer.indexBuffer.getPixel(new Int2(mousePosition)).x - 1;
            editor.scene.selectedEntity = (objectIndex >= 0) ? editor.scene.get(objectIndex) : null;
        }
    }

    private void displayFrame(Int2 viewportSize) {
        ImGui.image(editor.renderer.renderBuffer.getColorMap().getBuffer(), viewportSize.x, viewportSize.y, 0, 1, 1, 0);
    }

    private Pair<Integer> updateGizmoState() {
        int gizmoOperation = (int) editor.savedData.get("GizmoOperation");
        if (ImGui.isKeyPressed('1')) {
            gizmoOperation = (gizmoOperation != Operation.SCALE) ? Operation.SCALE : 0;
        }
        if (ImGui.isKeyPressed('2')) {
            gizmoOperation = (gizmoOperation != Operation.ROTATE) ? Operation.ROTATE : 0;
        }
        if (ImGui.isKeyPressed('3')) {
            gizmoOperation = (gizmoOperation != Operation.TRANSLATE) ? Operation.TRANSLATE : 0;
        }
        editor.savedData.put("GizmoOperation", gizmoOperation);
        return new Pair<>(gizmoOperation, Mode.WORLD); // Maybe future change
    }

    private void renderGizmos(Int2 viewportPosition, Int2 viewportSize, int gizmoOperation, int gizmoMode) {
        ImGuizmo.setEnabled(true);
        ImGuizmo.setDrawList();
        ImGuizmo.setRect(viewportPosition.x, viewportPosition.y, viewportSize.x, viewportSize.y);

        Entity selected = editor.scene.selectedEntity;
        if (selected == null || gizmoOperation == 0) {
            float[] ignored = new float[16];
            ImGuizmo.manipulate(ignored, ignored, ignored, gizmoOperation, gizmoMode);
            return;
        }

        TransformComponent transform = selected.transformComponent;
        Mat4 viewMatrix = editor.renderer.camera.viewMatrix().transpose();
        Mat4 projectionMatrix = editor.renderer.camera.projectionMatrix().transpose();
        Mat4 transformMatrix = transform.translationMatrix().mul(transform.scalingMatrix()).transpose();

        ImGuizmo.manipulate(viewMatrix.data, projectionMatrix.data, transformMatrix.data, gizmoOperation, gizmoMode);

        if (ImGuizmo.isUsing()) {
            float[][] data = new float[3][3];
            ImGuizmo.decomposeMatrixToComponents(transformMatrix.data, data[2], data[1], data[0]);

            transform.scale = new Float3(data[0]);
            transform.rotation = transform.rotation.add(new Float3(data[1]));
            transform.position = new Float3(data[2]);
        }
    }

    @Override
    public void renderGUI() {
        ImGui.pushStyleVar(ImGuiStyleVar.WindowPadding, 0, 0);

        if (ImGui.begin("Viewport", ImGuiWindowFlags.NoScrollbar)) {
            Pair<Int2> viewportInfo = saveViewportInfo();
            Pair<Integer> gizmoInfo = updateGizmoState();

            objectSelection(viewportInfo.first, viewportInfo.second);
            displayFrame(viewportInfo.second);
            renderGizmos(viewportInfo.first, viewportInfo.second, gizmoInfo.first, gizmoInfo.second);
        }
        ImGui.end();

        ImGui.popStyleVar();
    }
}
