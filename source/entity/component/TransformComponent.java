package entity.component;

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
        return translationMatrix().mul(rotationMatrix()).mul(scalingMatrix());
    }

    @Override
    public void renderGUI() {
        if (scale != null) {
            float[] scaleData = scale.array();
            ImGui.dragFloat3("Scale", scaleData, 0.1f);
            scale = new Float3(scaleData);
        }

        if (rotation != null) {
            float[] rotationData = rotation.array();
            ImGui.dragFloat3("Rotation", rotationData, 0.1f);
            rotation = new Float3(rotationData);
        }

        if (position != null) {
            float[] positionData = position.array();
            ImGui.dragFloat3("Position", positionData, 0.1f);
            position = new Float3(positionData);
        }
    }
}
