package entity;

import gui.GUIRenderable;
import math.Float3;
import math.Mat4;

import java.io.Serializable;

public class TransformComponent implements GUIRenderable, Serializable {
    public Float3 scale = new Float3(1);
    public Float3 rotation = new Float3();
    public Float3 position = new Float3();

    public TransformComponent() {}

    public Mat4 matrix() {
        return Mat4.translate(position).mul(Mat4.rotate(rotation)).mul(Mat4.scale(scale));
    }

    @Override
    public void onGUIRender() {

    }
}
