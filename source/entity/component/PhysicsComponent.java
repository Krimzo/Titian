package entity.component;

import gui.GUIUtil;
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
            GUIUtil.editFloat3("Acceleration", acceleration, 0.1f);
        }

        if (velocity != null) {
            GUIUtil.editFloat3("Velocity", velocity, 0.1f);
        }

        if (angular != null) {
            GUIUtil.editFloat3("Angular", angular, 0.1f);
        }
    }
}
