package gui.section;

import editor.Editor;
import entity.Entity;
import entity.component.TransformComponent;
import gui.abs.GUISection;
import gui.helper.GUIEdit;
import gui.helper.GUIStyle;
import imgui.ImGui;
import imgui.extension.imguizmo.ImGuizmo;
import imgui.extension.imguizmo.flag.Mode;
import imgui.extension.imguizmo.flag.Operation;
import imgui.flag.ImGuiMouseButton;
import imgui.flag.ImGuiStyleVar;
import imgui.flag.ImGuiWindowFlags;
import math.*;
import window.input.Key;
import window.input.Mouse;

public final class GUIViewport extends GUISection {
    private Int2 viewportPosition = new Int2();
    private Int2 viewportSize = new Int2();
    private int gizmoOperation = 0;
    private int gizmoMode = Mode.WORLD;

    public GUIViewport(Editor editor) {
        super(editor);
    }

    @Override
    public void dispose() {

    }

    private void updateViewport() {
        if (ImGui.isWindowFocused()) {
            editor.camera.useDefaultMovement(Mouse.Right, Key.W, Key.S, Key.D, Key.A, Key.E, Key.Q, 2,
                editor.window.getSize(), editor.timer.getDeltaT()
            );
        }

        final int tabSize = (int) ImGui.getWindowContentRegionMinY();
        viewportPosition = new Int2((int) ImGui.getWindowPosX(), (int) ImGui.getWindowPosY() + tabSize);
        viewportSize = new Int2((int) ImGui.getWindowWidth(), (int) ImGui.getWindowHeight() - tabSize);
    }

    private void renderViewportInfo() {
        ImGui.pushStyleVar(ImGuiStyleVar.WindowPadding, 14, 14);

        if (ImGui.begin("Viewport Info")) {
            ImGui.bulletText("Camera");

            GUIEdit.editFloat3("##Position", editor.camera.components.transform.position, 0.1f);

            float[] data = editor.camera.getForward().array();
            if (ImGui.dragFloat3("##Forward", data, 0.05f)) {
                editor.camera.setForward(new Float3(data));
            }
        }
        ImGui.end();

        ImGui.popStyleVar();
    }

    private void updateGizmoState() {
        if (!ImGui.isWindowFocused()) {
            return;
        }

        if (ImGui.isKeyPressed(Key.Num1)) {
            gizmoOperation = (gizmoOperation != Operation.SCALE) ? Operation.SCALE : 0;
        }
        if (ImGui.isKeyPressed(Key.Num2)) {
            gizmoOperation = (gizmoOperation != Operation.ROTATE) ? Operation.ROTATE : 0;
        }
        if (ImGui.isKeyPressed(Key.Num3)) {
            gizmoOperation = (gizmoOperation != Operation.TRANSLATE) ? Operation.TRANSLATE : 0;
        }
        if (ImGui.isKeyPressed(Key.Num4)) {
            gizmoMode = (gizmoMode == Mode.WORLD) ? Mode.LOCAL : Mode.WORLD;
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

            int objectIndex = (int) editor.editorRenderer.indexBuffer.getPixel(new Int2(mousePosition)).x - 1;
            editor.scene.selected = (objectIndex >= 0) ? editor.scene.get(objectIndex) : null;
        }
    }

    private void renderScene() {
        editor.editorRenderer.clear(editor.camera);

        if (editor.scene == null) {
            return;
        }

        int selectedIndex = editor.scene.indexOf(editor.scene.selected);

        editor.window.getContext().setViewport(viewportSize);
        editor.editorRenderer.resize(viewportSize);
        editor.camera.updateAspect(viewportSize);

        editor.window.getContext().setWireframe(editor.data.wireframeState);

        editor.editorRenderer.renderScene(editor.scene, editor.camera);
        editor.editorRenderer.renderIndices(editor.scene, editor.camera);

        editor.window.getContext().setWireframe(false);

        if (selectedIndex >= 0) {
            editor.editorRenderer.renderOutline(new Float2(viewportSize), GUIStyle.special, selectedIndex);
        }
    }

    private void displayFrame() {
        ImGui.image(editor.editorRenderer.renderBuffer.getColorMap().getBuffer(),
            viewportSize.x, viewportSize.y,
            0, 1,
            1, 0
        );
    }

    private void renderGizmos() {
        if (editor.scene == null) {
            return;
        }

        ImGuizmo.setEnabled(true);
        ImGuizmo.setDrawList();
        ImGuizmo.setRect(viewportPosition.x, viewportPosition.y, viewportSize.x, viewportSize.y);

        Entity selected = editor.scene.selected;
        if (selected == null || gizmoOperation == 0) {
            float[] ignored = new float[16];
            ImGuizmo.manipulate(ignored, ignored, ignored, gizmoOperation, gizmoMode);
            return;
        }

        TransformComponent transform = selected.components.transform;
        Mat4 viewMatrix = editor.camera.viewMatrix().transpose();
        Mat4 projectionMatrix = editor.camera.projectionMatrix().transpose();
        Mat4 transformMatrix = transform.matrix().transpose();

        float[][] result = new float[2][16];
        ImGuizmo.manipulate(viewMatrix.data, projectionMatrix.data, transformMatrix.data, result[0],
            gizmoOperation, gizmoMode, result[1], result[1], result[1]
        );

        if (ImGuizmo.isUsing()) {
            float[][] decomposed = new float[3][3];

            ImGuizmo.decomposeMatrixToComponents(transformMatrix.data, decomposed[2], decomposed[1], decomposed[0]);
            transform.scale.set(new Float3(decomposed[0]));
            transform.position.set(new Float3(decomposed[2]));

            ImGuizmo.decomposeMatrixToComponents(result[0], decomposed[2], decomposed[1], decomposed[0]);
            transform.rotation.set(transform.rotation.add(new Float3(decomposed[1])));
        }
    }

    @Override
    public void renderGUI() {
        ImGui.pushStyleVar(ImGuiStyleVar.WindowPadding, 0, 0);

        ImGui.setNextWindowBgAlpha(1);
        if (ImGui.begin("Viewport", ImGuiWindowFlags.NoScrollbar)) {
            updateViewport();
            renderViewportInfo();

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
