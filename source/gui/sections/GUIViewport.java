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
        if (editor.getScene() == null || ImGuizmo.isOver()) {
            return;
        }

        Float4 viewportRect = new Float4(
            viewportPosition.x, viewportPosition.y, viewportPosition.x + viewportSize.x, viewportPosition.y + viewportSize.y
        );

        if (ImGui.isMouseHoveringRect(viewportRect.x, viewportRect.y, viewportRect.z, viewportRect.w) && ImGui.isMouseClicked(ImGuiMouseButton.Left)) {
            Float2 mousePosition = new Float2(ImGui.getMousePosX() - viewportPosition.x, ImGui.getMousePosY() - viewportPosition.y);

            int objectIndex = (int) editor.renderer.indexBuffer.getPixel(new Int2(mousePosition)).x - 1;
            editor.getScene().selectedEntity = (objectIndex >= 0) ? editor.getScene().get(objectIndex) : null;
        }
    }

    private void displayFrame(Int2 viewportSize) {
        ImGui.image(editor.renderer.renderBuffer.getColorMap().getBuffer(), viewportSize.x, viewportSize.y, 0, 1, 1, 0);
    }

    private int updateGizmoState() {
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
        return gizmoOperation;
    }

    private void renderGizmos(Int2 viewportPosition, Int2 viewportSize, int gizmoOperation) {
        ImGuizmo.setEnabled(true);
        ImGuizmo.setDrawList();
        ImGuizmo.setRect(viewportPosition.x, viewportPosition.y, viewportSize.x, viewportSize.y);

        Entity selected = editor.getScene().selectedEntity;
        if (selected == null || gizmoOperation == 0) {
            float[] ignored = new float[16];
            ImGuizmo.manipulate(ignored, ignored, ignored, Operation.TRANSLATE, Mode.WORLD);
            return;
        }

        Mat4 viewMatrix = editor.renderer.camera.viewMatrix().transpose();
        Mat4 projectionMatrix = editor.renderer.camera.projectionMatrix().transpose();
        Mat4 transformMatrix = selected.transformComponent.matrix().transpose();

        ImGuizmo.manipulate(viewMatrix.data, projectionMatrix.data, transformMatrix.data, gizmoOperation, Mode.WORLD);

        if (ImGuizmo.isUsing()) {
            float[] data = transformMatrix.data;

            Float3 scale = new Float3(1);
            scale.x = new Float3(data[0], data[1], data[2]).len();
            scale.y = new Float3(data[4], data[5], data[6]).len();
            scale.z = new Float3(data[8], data[9], data[10]).len();

            Float3 rotation = new Float3();
            rotation.x = (float) Math.toDegrees(Math.atan2(data[6], data[10]));
            rotation.y = (float) Math.toDegrees(Math.atan2(-data[2], Math.sqrt(data[6] * data[6] + data[10] * data[10])));
            rotation.z = (float) Math.toDegrees(Math.atan2(data[1], data[0]));

            Float3 position = new Float3();
            position.x = data[12];
            position.y = data[13];
            position.z = data[14];

            selected.transformComponent.scale = scale;
            //selected.transformComponent.rotation = rotation; WIP
            selected.transformComponent.position = position;
        }
    }

    @Override
    public void renderGUI() {
        ImGui.pushStyleVar(ImGuiStyleVar.WindowPadding, 0, 0);

        if (ImGui.begin("Viewport", ImGuiWindowFlags.NoScrollbar)) {
            Pair<Int2> viewportInfo = saveViewportInfo();
            int gizmoOperation = updateGizmoState();

            objectSelection(viewportInfo.first, viewportInfo.second);
            displayFrame(viewportInfo.second);
            renderGizmos(viewportInfo.first, viewportInfo.second, gizmoOperation);
        }
        ImGui.end();

        ImGui.popStyleVar();
    }
}
