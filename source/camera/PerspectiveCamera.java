package camera;

import camera.abs.Camera;
import math.*;
import named.NameHolder;

import java.io.Serializable;

public class PerspectiveCamera extends Camera implements Serializable {
    public float aspect = 1.78f;
    public float fov = 75;

    public PerspectiveCamera(NameHolder holder, String name) {
        super(holder, name);
    }

    @Override
    public Mat4 viewMatrix() {
        return Mat4.lookAt(transformComponent.position, transformComponent.position.add(forward), Float3.getPosY());
    }

    @Override
    public Mat4 projectionMatrix() {
        return Mat4.perspective(fov, aspect, near, far);
    }

    @Override
    public Mat4 matrix() {
        return projectionMatrix().mul(viewMatrix());
    }

    public void updateAspect(Int2 frameSize) {
        aspect = ((float) frameSize.x) / frameSize.y;
    }
}
