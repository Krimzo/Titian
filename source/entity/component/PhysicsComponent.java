package entity.component;

import gui.abs.GUIRenderable;
import imgui.ImGui;
import math.Float3;

import java.io.Serializable;

public class PhysicsComponent implements GUIRenderable, Serializable {
    public Float3 acceleration = new Float3();
    public Float3 velocity = new Float3();
    public Float3 angular = new Float3();

    public PhysicsComponent() {}

    @Override
    public void renderGUI() {
        if (acceleration != null) {
            float[] accelerationData = acceleration.array();
            ImGui.dragFloat3("Acceleration", accelerationData, 0.1f);
            acceleration = new Float3(accelerationData);
        }

        if (velocity != null) {
            float[] velocityData = velocity.array();
            ImGui.dragFloat3("Velocity", velocityData, 0.1f);
            velocity = new Float3(velocityData);
        }

        if (angular != null) {
            float[] angularData = angular.array();
            ImGui.dragFloat3("Angular", angularData, 0.1f);
            angular = new Float3(angularData);
        }
    }
}
