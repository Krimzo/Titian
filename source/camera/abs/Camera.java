package camera.abs;

import editor.Editor;
import entity.Entity;
import gui.GUIUtil;
import imgui.ImGui;
import math.*;
import named.NameHolder;
import window.input.Input;
import window.input.Mouse;

import java.io.Serializable;

public abstract class Camera extends Entity implements Serializable {
    protected Float3 forward = Float3.getNegZ();

    public float near = 0.01f;
    public float far = 500;

    public float speed = 2;
    public float sensitivity = 0.1f;

    public Float3 background = new Float3(0.1f);

    private boolean firstClick = true;
    private boolean camMoving = false;

    public Camera(NameHolder holder, String name, Editor editor) {
        super(holder, name, editor);
    }

    public Mat4 viewMatrix() {
        return Mat4.lookAt(components.transform.position, components.transform.position.add(forward), Float3.getPosY());
    }

    public abstract Mat4 projectionMatrix();
    public abstract Mat4 matrix();

    public void setForward(Float3 forward) {
        this.forward = forward.normalize();
    }

    public Float3 getForward() {
        return new Float3(forward);
    }

    public Float3 getRight() {
        return forward.cross(Float3.getPosY());
    }

    public void moveForward(float deltaTime) {
        components.transform.position.set(components.transform.position.add(forward.multiply(speed * deltaTime)));
    }

    public void moveBack(float deltaTime) {
        components.transform.position.set(components.transform.position.subtract(forward.multiply(speed * deltaTime)));
    }

    public void moveRight(float deltaTime) {
        components.transform.position.set(components.transform.position.add(getRight().multiply(speed * deltaTime)));
    }

    public void moveLeft(float deltaTime) {
        components.transform.position.set(components.transform.position.subtract(getRight().multiply(speed * deltaTime)));
    }

    public void moveUp(float deltaTime) {
        components.transform.position.set(components.transform.position.add(Float3.getPosY().multiply(speed * deltaTime)));
    }

    public void moveDown(float deltaTime) {
        components.transform.position.set(components.transform.position.subtract(Float3.getPosY().multiply(speed * deltaTime)));
    }

    public void rotate(Int2 mousePos, Int2 frameCenter, float verticalAngleLimit) {
        final Int2 delta = mousePos.subtract(frameCenter);
        final Float2 rotation = new Float2(delta).multiply(sensitivity);

        Float3 forwardVert = forward.rotate(-rotation.y, getRight());
        if (Math.abs(forwardVert.angle(Float3.getPosY()) - 90.0f) <= verticalAngleLimit) {
            forward = forwardVert;
        }

        forward = forward.rotate(-rotation.x, Float3.getPosY());
    }

    public void useDefaultMovement(int forward, int back, int right, int left, int up, int down, Int2 frameSize, float deltaT) {
        speed = Input.isShiftDown() ? 5 : 2;

        if (Input.isKeyDown(forward)) {
            moveForward(deltaT);
        }
        if (Input.isKeyDown(back)) {
            moveBack(deltaT);
        }
        if (Input.isKeyDown(right)) {
            moveRight(deltaT);
        }
        if (Input.isKeyDown(left)) {
            moveLeft(deltaT);
        }
        if (Input.isKeyDown(up)) {
            moveUp(deltaT);
        }
        if (Input.isKeyDown(down)) {
            moveDown(deltaT);
        }

        if (Input.isMousePressed(Mouse.Middle)) {
            editor.window.setMouseState(false);
            camMoving = true;
        }

        if (Input.isMouseDown(Mouse.Middle)) {
            if (camMoving) {
                final Int2 frameCenter = frameSize.divide(2);

                if (!firstClick) {
                    rotate(editor.window.getMousePosition(), frameCenter, 85);
                }
                firstClick = false;

                editor.window.setMousePosition(frameCenter);
            }
        }

        if (Input.isMouseReleased(Mouse.Middle)) {
            editor.window.setMouseState(true);
            firstClick = true;
            camMoving = false;
        }
    }

    @Override
    public void renderInfoGUI(Editor editor) {
        super.renderInfoGUI(editor);

        boolean isMainCamera = editor.scene.camera == this;
        if (ImGui.checkbox("Main camera", isMainCamera)) {
            editor.scene.camera = isMainCamera ? null : this;
        }

        GUIUtil.editFloat3("Forward", forward, 0.01f);
        setForward(forward);

        near = GUIUtil.editFloat("Near plane", near, 0.05f);
        far = GUIUtil.editFloat("Far plane", far, 0.05f);

        speed = GUIUtil.editFloat("Speed", speed, 0.05f);
        sensitivity = GUIUtil.editFloat("Sensitivity", sensitivity, 0.05f);

        GUIUtil.editColor3("Background", background);
    }
}
