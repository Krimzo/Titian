package renderer;

import imgui.ImGui;
import math.*;
import utility.Timer;
import window.Window;

public class Camera {
    public Float3 position = new Float3();
    public float aspect = 16.0f / 9.0f;
    public float fov = 75.0f;
    public float near = 0.01f;
    public float far = 500.0f;
    public float speed = 2.0f;
    public float sensitivity = 0.1f;

    private Float3 forward = Float3.getNegZ();
    private boolean firstClick = true;
    private boolean camMoving = false;

    public Camera() {}

    public void updateAspect(Int2 frameSize) {
        aspect = ((float) frameSize.x) / frameSize.y;
    }

    public void setForward(Float3 forward) {
        this.forward = forward.norm();
    }

    public Float3 getForward() {
        return new Float3(forward);
    }

    public Float3 getRight() {
        return forward.cross(Float3.getPosY());
    }

    public void moveForward(float deltaTime) {
        position = position.add(forward.mul(speed * deltaTime));
    }

    public void moveBack(float deltaTime) {
        position = position.sub(forward.mul(speed * deltaTime));
    }

    public void moveRight(float deltaTime) {
        position = position.add(getRight().mul(speed * deltaTime));
    }

    public void moveLeft(float deltaTime) {
        position = position.sub(getRight().mul(speed * deltaTime));
    }

    public void moveUp(float deltaTime) {
        position = position.add(Float3.getPosY().mul(speed * deltaTime));
    }

    public void moveDown(float deltaTime) {
        position = position.sub(Float3.getPosY().mul(speed * deltaTime));
    }

    public void rotate(Int2 mousePos, Int2 frameCenter, float verticalAngleLimit) {
	    final Int2 delta = mousePos.sub(frameCenter);
        final Float2 rotation = new Float2(delta).mul(sensitivity);

        Float3 forwardVert = forward.rotate(-rotation.y, getRight());
        if (Math.abs(forwardVert.angle(Float3.getPosY()) - 90.0f) <= verticalAngleLimit) {
            forward = forwardVert;
        }

        forward = forward.rotate(-rotation.x, Float3.getPosY());
    }

    public Mat4 viewMatrix() {
        return Mat4.lookAt(position, position.add(forward), Float3.getPosY());
    }

    public Mat4 projectionMatrix() {
        return Mat4.perspective(fov, aspect, near, far);
    }

    public Mat4 matrix() {
        return projectionMatrix().mul(viewMatrix());
    }

    public void setDefaultMovement(Window window, Timer timer) {
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
			    final Int2 frameCenter = window.getSize().div(2);

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
}
