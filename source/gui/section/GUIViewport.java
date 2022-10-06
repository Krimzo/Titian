package gui.section;

import editor.*;
import entity.*;
import entity.component.*;
import gui.abs.GUISection;
import imgui.*;
import imgui.extension.imguizmo.*;
import imgui.extension.imguizmo.flag.*;
import imgui.flag.*;
import math.*;

public final class GUIViewport extends GUISection {
    private Int2 viewportPosition = new Int2();
    private Int2 viewportSize = new Int2();
    private int gizmoOperation = 0;
    private int gizmoMode = Mode.WORLD;

    public GUIViewport(Editor editor) {
        super(editor);
    }

    private void updateViewportInfo() {
        final int tabSize = (int) ImGui.getWindowContentRegionMinY();
        viewportPosition = new Int2((int) ImGui.getWindowPosX(), (int) ImGui.getWindowPosY() + tabSize);
        viewportSize = new Int2((int) ImGui.getWindowWidth(), (int) ImGui.getWindowHeight() - tabSize);
    }

    private void updateGizmoState() {
        if (ImGui.isKeyPressed('1')) {
            gizmoOperation = (gizmoOperation != Operation.SCALE) ? Operation.SCALE : 0;
        }
        if (ImGui.isKeyPressed('2')) {
            gizmoOperation = (gizmoOperation != Operation.ROTATE) ? Operation.ROTATE : 0;
        }
        if (ImGui.isKeyPressed('3')) {
            gizmoOperation = (gizmoOperation != Operation.TRANSLATE) ? Operation.TRANSLATE : 0;
        }
    }

    private void objectSelection() {
        if (editor.scene == null || ImGuizmo.isOver()) {
            return;
        }

        Float4 viewportRect = new Float4(
            viewportPosition.x, viewportPosition.y, viewportPosition.x + viewportSize.x, viewportPosition.y + viewportSize.y
        );

        if (ImGui.isMouseHoveringRect(viewportRect.x, viewportRect.y, viewportRect.z, viewportRect.w) && ImGui.isMouseClicked(ImGuiMouseButton.Left)) {
            Float2 mousePosition = new Float2(ImGui.getMousePosX() - viewportPosition.x, ImGui.getMousePosY() - viewportPosition.y);

            int objectIndex = (int) editor.viewportRenderer.indexBuffer.getPixel(new Int2(mousePosition)).x - 1;
            editor.scene.selectedEntity = (objectIndex >= 0) ? editor.scene.get(objectIndex) : null;
        }
    }

    private void renderScene() {
        editor.viewportRenderer.renderBuffer.context.setClearColor(new Float4(0.2f));
        editor.viewportRenderer.renderBuffer.clear();

        if (editor.scene == null) {
            return;
        }

        int selectedIndex = editor.scene.indexOf(editor.scene.selectedEntity);

        editor.window.getContext().setViewport(viewportSize);
        editor.viewportRenderer.resize(viewportSize);
        editor.camera.updateAspect(viewportSize);

        editor.window.getContext().setWireframe(editor.wireframeState);
        editor.viewportRenderer.renderScene(editor.scene, editor.camera);
        editor.viewportRenderer.renderIndices(editor.scene, editor.camera);
        editor.window.getContext().setWireframe(false);

        if (selectedIndex >= 0) {
            editor.viewportRenderer.renderOutline(new Float2(viewportSize), editor.outlineColor, selectedIndex);
        }
    }

    private void displayFrame() {
        ImGui.image(editor.viewportRenderer.renderBuffer.getColorMap().getBuffer(), viewportSize.x, viewportSize.y, 0, 1, 1, 0);
    }

    private void renderGizmos() {
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
        Mat4 viewMatrix = editor.camera.viewMatrix().transpose();
        Mat4 projectionMatrix = editor.camera.projectionMatrix().transpose();
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
            updateViewportInfo();
            updateGizmoState();
            objectSelection();

            renderScene();
            displayFrame();
            renderGizmos();
        }
        ImGui.end();

        ImGui.popStyleVar();
    }
}
