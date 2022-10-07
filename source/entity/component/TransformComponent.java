package entity.component;

import gui.GUIUtil;
import gui.abs.GUIRenderable;
import imgui.ImGui;
import math.Float3;
import math.Mat4;

import java.io.Serializable;

public class TransformComponent implements GUIRenderable, Serializable {
    public Float3 scale = new Float3(1);
    public Float3 rotation = new Float3();
    public Float3 position = new Float3();

    public TransformComponent() {}

    public Mat4 scalingMatrix() {
        if (scale != null) {
            return Mat4.scaling(scale);
        }
        return new Mat4();
    }

    public Mat4 rotationMatrix() {
        if (rotation != null) {
            return Mat4.rotation(rotation);
        }
        return new Mat4();
    }

    public Mat4 translationMatrix() {
        if (position != null) {
            return Mat4.translation(position);
        }
        return new Mat4();
    }

    public Mat4 matrix() {
        return translationMatrix().multiply(rotationMatrix()).multiply(scalingMatrix());
    }

    @Override
    public void renderGUI() {
        if (scale != null) {
            GUIUtil.editFloat3("Scale", scale, 0.1f);
        }

        if (rotation != null) {
            GUIUtil.editFloat3("Rotation", rotation, 0.1f);
        }

        if (position != null) {
            GUIUtil.editFloat3("Position", position, 0.1f);
        }
    }
}
