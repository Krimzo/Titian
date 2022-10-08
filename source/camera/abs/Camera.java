package camera.abs;

import editor.Editor;
import entity.Entity;
import gui.GUIUtil;
import imgui.ImGui;
import math.*;
import named.NameHolder;
import utility.Timer;
import window.Window;

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

    public abstract Mat4 viewMatrix();
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
        transformComponent.position = transformComponent.position.add(forward.multiply(speed * deltaTime));
    }

    public void moveBack(float deltaTime) {
        transformComponent.position = transformComponent.position.subtract(forward.multiply(speed * deltaTime));
    }

    public void moveRight(float deltaTime) {
        transformComponent.position = transformComponent.position.add(getRight().multiply(speed * deltaTime));
    }

    public void moveLeft(float deltaTime) {
        transformComponent.position = transformComponent.position.subtract(getRight().multiply(speed * deltaTime));
    }

    public void moveUp(float deltaTime) {
        transformComponent.position = transformComponent.position.add(Float3.getPosY().multiply(speed * deltaTime));
    }

    public void moveDown(float deltaTime) {
        transformComponent.position = transformComponent.position.subtract(Float3.getPosY().multiply(speed * deltaTime));
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

    public void setupDefaultMovement(Window window, Timer timer) {
        window.keyboard.w.onHold.add(() -> moveForward(timer.getDeltaT()));
        window.keyboard.a.onHold.add(() -> moveLeft(timer.getDeltaT()));
        window.keyboard.s.onHold.add(() -> moveBack(timer.getDeltaT()));
        window.keyboard.d.onHold.add(() -> moveRight(timer.getDeltaT()));
        window.keyboard.e.onHold.add(() -> moveUp(timer.getDeltaT()));
        window.keyboard.q.onHold.add(() -> moveDown(timer.getDeltaT()));

        window.keyboard.shift.onPress.add(() -> speed = 5);
        window.keyboard.shift.onRelease.add(() -> speed = 2);

        window.mouse.mmb.onPress.add(() -> {
            window.mouse.setHidden(true);
            camMoving = true;
        });

        window.mouse.mmb.onHold.add(() -> {
            if (camMoving) {
                final Int2 frameCenter = window.getSize().divide(2);

                if (!firstClick) {
                    rotate(window.mouse.getPosition(), frameCenter, 85);
                }
                firstClick = false;

                window.mouse.setPosition(frameCenter);
            }
        });

        window.mouse.mmb.onRelease.add(() -> {
            window.mouse.setHidden(false);
            firstClick = true;
            camMoving = false;
        });
    }

    @Override
    public void renderInfoGUI(Editor editor) {
        super.renderInfoGUI(editor);

        boolean isMainCamera = editor.scene.mainCamera == this;
        if (ImGui.checkbox("Main camera", isMainCamera)) {
            editor.scene.mainCamera = isMainCamera ? null : this;
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
