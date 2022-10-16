package gui.section;

import editor.Editor;
import entity.Entity;
import entity.component.TransformComponent;
import glparts.Mesh;
import glparts.Shaders;
import gui.abs.GUISection;
import gui.helper.GUIDisplay;
import gui.helper.GUIDragDrop;
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
import scene.Scene;
import utility.Instance;
import window.input.Input;
import window.input.Key;
import window.input.Mouse;

public final class GUIViewport extends GUISection {
    private Int2 viewportPosition = new Int2();
    private Int2 viewportSize = new Int2();

    private Mesh gridMesh = null;
    private Shaders gridShaders = null;

    private final float[][] snap = {
        { 0.1f, 0.1f, 0.1f },
        { 30, 30, 30 },
        { 1, 1, 1 },
        { 0, 0, 0 },
    };

    public GUIViewport(Editor editor) {
        super(editor);

        gridMesh = Mesh.generateGridMesh(50);
        gridShaders = new Shaders(editor.window.getContext(), "shaders/Grid.glsl");
    }

    private void updateViewport() {
        final int tabSize = (int) ImGui.getWindowContentRegionMinY();
        viewportPosition = new Int2((int) ImGui.getWindowPosX(), (int) ImGui.getWindowPosY() + tabSize);
        viewportSize = new Int2((int) ImGui.getWindowWidth(), (int) ImGui.getWindowHeight() - tabSize);
    }

    private void renderViewportInfo() {
        ImGui.pushStyleVar(ImGuiStyleVar.WindowPadding, 14, 14);

        if (ImGui.begin("Viewport Info")) {
            ImGui.bulletText("Camera");

            GUIEdit.editFloat3("Position", editor.camera.components.transform.position, 0.1f);

            float[] data = editor.camera.getForward().array();
            if (ImGui.dragFloat3("Forward", data, 0.05f)) {
                editor.camera.setForward(new Float3(data));
            }
        }
        ImGui.end();

        ImGui.popStyleVar();
    }

    private void updateCamera() {
        editor.camera.useDefaultMovement(Mouse.Right, Key.W, Key.S, Key.D, Key.A, Key.E, Key.Q, 2,
            editor.window.getSize(), editor.timer.getDeltaT()
        );
    }

    private void updateGizmoState() {
        if (ImGui.isKeyPressed(Key.Num1)) {
            editor.data.gizmoOperation = (editor.data.gizmoOperation != Operation.SCALE) ? Operation.SCALE : 0;
        }
        if (ImGui.isKeyPressed(Key.Num2)) {
            editor.data.gizmoOperation = (editor.data.gizmoOperation != Operation.ROTATE) ? Operation.ROTATE : 0;
        }
        if (ImGui.isKeyPressed(Key.Num3)) {
            editor.data.gizmoOperation = (editor.data.gizmoOperation != Operation.TRANSLATE) ? Operation.TRANSLATE : 0;
        }
        if (ImGui.isKeyPressed(Key.Num4)) {
            editor.data.gizmoMode = (editor.data.gizmoMode == Mode.WORLD) ? Mode.LOCAL : Mode.WORLD;
        }
    }

    private void objectSelection() {
        if (ImGuizmo.isOver()) {
            return;
        }

        Float4 viewportRect = new Float4(
            viewportPosition.x, viewportPosition.y, viewportPosition.x + viewportSize.x, viewportPosition.y + viewportSize.y
        );

        if (ImGui.isMouseHoveringRect(viewportRect.x, viewportRect.y, viewportRect.z, viewportRect.w) && ImGui.isMouseClicked(ImGuiMouseButton.Left)) {
            Float2 mousePosition = new Float2(ImGui.getMousePosX() - viewportPosition.x, ImGui.getMousePosY() - viewportPosition.y);

            int objectIndex = (int) editor.editorRenderer.indexBuffer.getPixel(new Int2(mousePosition)).x - 1;
            editor.getScene().selected.entity = (objectIndex >= 0) ? editor.getScene().get(objectIndex) : null;
        }
    }

    private void renderScene() {
        editor.editorRenderer.clear(editor.camera);

        editor.window.getContext().setViewport(viewportSize);
        editor.editorRenderer.resize(viewportSize);
        editor.camera.updateAspect(viewportSize);

        if (editor.data.renderGrid) {
            editor.editorRenderer.renderBuffer.use(() -> {
                Float3 position = editor.camera.components.transform.position;
                position = new Float3((int) position.x, 0, (int) position.z);
                gridShaders.setUniform("W", Mat4.translation(position));
                gridShaders.setUniform("VP", editor.camera.matrix());
                gridMesh.renderLines(gridShaders);
            });
        }

        int selectedIndex = editor.getScene().indexOf(editor.getScene().selected.entity);

        editor.window.getContext().setWireframe(editor.data.wireframeState);
        editor.editorRenderer.renderScene(editor.getScene(), editor.camera);
        editor.editorRenderer.renderIndices(editor.getScene(), editor.camera);
        editor.window.getContext().setWireframe(false);

        if (selectedIndex >= 0) {
            editor.editorRenderer.renderOutline(new Float2(viewportSize), GUIStyle.special, selectedIndex);
        }
    }

    private void renderGizmos() {
        ImGuizmo.setEnabled(true);
        ImGuizmo.setDrawList();
        ImGuizmo.setRect(viewportPosition.x, viewportPosition.y, viewportSize.x, viewportSize.y);

        Entity selected = editor.getScene().selected.entity;
        if (Instance.isNull(selected) || editor.data.gizmoOperation == 0) {
            float[] ignored = new float[16];
            ImGuizmo.manipulate(ignored, ignored, ignored, editor.data.gizmoOperation, editor.data.gizmoMode);
            return;
        }

        TransformComponent transform = selected.components.transform;
        Mat4 viewMatrix = editor.camera.viewMatrix().transpose();
        Mat4 projectionMatrix = editor.camera.projectionMatrix().transpose();
        Mat4 transformMatrix = transform.matrix().transpose();

        float[] snap = this.snap[3];
        if (Input.isShiftDown()) {
            switch (editor.data.gizmoOperation) {
                case Operation.SCALE -> snap = this.snap[0];
                case Operation.ROTATE -> snap = this.snap[1];
                case Operation.TRANSLATE -> snap = this.snap[2];
            }
        }

        float[] result = new float[16];
        ImGuizmo.manipulate(viewMatrix.data, projectionMatrix.data, transformMatrix.data, result,
            editor.data.gizmoOperation, editor.data.gizmoMode, snap, this.snap[3], this.snap[3]
        );

        if (ImGuizmo.isUsing()) {
            float[][] decomposed = new float[3][3];

            ImGuizmo.decomposeMatrixToComponents(transformMatrix.data, decomposed[2], decomposed[1], decomposed[0]);
            transform.scale.set(new Float3(decomposed[0]));
            transform.position.set(new Float3(decomposed[2]));

            ImGuizmo.decomposeMatrixToComponents(result, decomposed[2], decomposed[1], decomposed[0]);
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

            if (ImGui.isWindowFocused()) {
                updateCamera();
                updateGizmoState();
                objectSelection();
            }

            renderScene();
            GUIDisplay.texture(editor.editorRenderer.renderBuffer.getColorMap(), viewportSize);
            GUIDragDrop.getData("SceneFile", path -> {
                if (editor.setScene(Scene.fromFile((String) path, editor))) {
                    System.out.println("Scene \"" + path + "\" loaded!");
                }
            });

            renderGizmos();
        }
        ImGui.end();

        ImGui.popStyleVar();
    }
}
