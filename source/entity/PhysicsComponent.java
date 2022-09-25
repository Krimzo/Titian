package entity;

import gui.GUIRenderable;
import math.Float3;

import java.io.Serializable;

public class PhysicsComponent implements GUIRenderable, Serializable {
    public Float3 acceleration = new Float3();
    public Float3 velocity = new Float3();
    public Float3 angular = new Float3();

    public PhysicsComponent() {}

    @Override
    public void onGUIRender() {

    }
}
