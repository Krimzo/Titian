package entity.component;

import entity.Entity;
import entity.abs.EntityComponent;
import gui.helper.GUIEdit;
import math.Float3;

import java.io.Serializable;

public class PhysicsComponent extends EntityComponent implements Serializable {
    public final Float3 acceleration = new Float3();
    public final Float3 velocity = new Float3();
    public final Float3 angular = new Float3();

    public PhysicsComponent(Entity entity) {
        super(entity);
    }

    @Override
    public void renderGUI() {
        GUIEdit.editFloat3("Acceleration", acceleration, 0.1f);
        GUIEdit.editFloat3("Velocity", velocity, 0.1f);
        GUIEdit.editFloat3("Angular", angular, 0.1f);
    }
}
