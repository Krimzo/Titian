package entity;

import gui.GUIRenderable;
import imgui.ImGui;
import imgui.flag.ImGuiWindowFlags;
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
        if (ImGui.begin("Transform", ImGuiWindowFlags.NoScrollbar)) {
            float[] scaleData = scale.array();
            ImGui.dragFloat3("Scale", scaleData, 0.1f);
            scale = new Float3(scaleData);

            float[] rotationData = rotation.array();
            ImGui.dragFloat3("Rotation", rotationData, 0.1f);
            rotation = new Float3(rotationData);

            float[] positionData = position.array();
            ImGui.dragFloat3("Position", positionData, 0.1f);
            position = new Float3(positionData);
        }
        ImGui.end();
    }
}
