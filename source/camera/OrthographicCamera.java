package camera;

import camera.abs.Camera;
import math.Float3;
import math.Mat4;
import named.NameHolder;

import java.io.Serializable;

public class OrthographicCamera extends Camera implements Serializable {
    public float width = 160;
    public float height = 90;

    public OrthographicCamera(NameHolder holder, String name) {
        super(holder, name);
    }

    @Override
    public Mat4 viewMatrix() {
        return Mat4.lookAt(transformComponent.position, transformComponent.position.add(forward), Float3.getPosY());
    }

    @Override
    public Mat4 projectionMatrix() {
        return Mat4.orthographic(width, height, near, far);
    }

    @Override
    public Mat4 matrix() {
        return projectionMatrix().mul(viewMatrix());
    }
}
