package camera.abs;

import editor.Editor;
import entity.Entity;
import gui.GUIUtil;
import imgui.ImGui;
import math.Float2;
import math.Float3;
import math.Int2;
import math.Mat4;
import named.NameHolder;
import window.input.Input;
import window.input.Mouse;

import java.io.Serializable;

public abstract class Camera extends Entity implements Serializable {
    private Float3 forward = Float3.getNegZ();
    private Float3 right = Float3.getPosX();
    private Float3 up = Float3.getPosY();

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

    public void setForward(Float3 forward) {
        this.forward = forward.normalize();
        right = this.forward.cross(up);
    }

    public void setUp(Float3 up) {
        this.up = up.normalize();
        right = forward.cross(this.up);
    }

    public Float3 getForward() {
        return new Float3(forward);
    }

    public Float3 getRight() {
        return new Float3(right);
    }

    public Float3 getUp() {
        return new Float3(up);
    }

    public Mat4 viewMatrix() {
        return Mat4.lookAt(components.transform.position, components.transform.position.add(forward), up);
    }

    public abstract Mat4 projectionMatrix();

    public abstract Mat4 matrix();

    public void moveForward(float deltaTime) {
        components.transform.position.set(components.transform.position.add(forward.multiply(speed * deltaTime)));
    }

    public void moveBack(float deltaTime) {
        components.transform.position.set(components.transform.position.subtract(forward.multiply(speed * deltaTime)));
    }

    public void moveRight(float deltaTime) {
        components.transform.position.set(components.transform.position.add(right.multiply(speed * deltaTime)));
    }

    public void moveLeft(float deltaTime) {
        components.transform.position.set(components.transform.position.subtract(right.multiply(speed * deltaTime)));
    }

    public void moveUp(float deltaTime) {
        components.transform.position.set(components.transform.position.add(up.multiply(speed * deltaTime)));
    }

    public void moveDown(float deltaTime) {
        components.transform.position.set(components.transform.position.subtract(up.multiply(speed * deltaTime)));
    }

    public void rotate(Int2 mousePos, Int2 frameCenter, float angleLimit) {
        final Int2 delta = mousePos.subtract(frameCenter);
        final Float2 rotation = new Float2(delta).multiply(sensitivity);

        Float3 forwardVert = forward.rotate(-rotation.y, right);
        if (Math.abs(forwardVert.angle(up) - 90) <= angleLimit) {
            forward = forwardVert;
        }

        setForward(forward.rotate(-rotation.x, up));
    }

    public void useDefaultMovement(int forward, int back, int right, int left, int up, int down, float multi, Int2 frameSize, float deltaT) {
        final float speed = this.speed;
        if (Input.isShiftDown()) {
            this.speed *= multi;
        }

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

        this.speed = speed;

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
