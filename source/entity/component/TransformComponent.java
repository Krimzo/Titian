package entity.component;

import entity.Entity;
import entity.abs.EntityComponent;
import gui.GUIEdit;
import math.Float3;
import math.Mat4;

import java.io.Serializable;

public class TransformComponent extends EntityComponent implements Serializable {
    public final Float3 scale = new Float3(1);
    public final Float3 rotation = new Float3();
    public final Float3 position = new Float3();

    public TransformComponent(Entity entity) {
        super(entity);
    }

    public Mat4 scalingMatrix() {
        return Mat4.scaling(scale);
    }

    public Mat4 rotationMatrix() {
        return Mat4.rotation(rotation);
    }

    public Mat4 translationMatrix() {
        return Mat4.translation(position);
    }

    public Mat4 matrix() {
        return translationMatrix().multiply(rotationMatrix()).multiply(scalingMatrix());
    }

    @Override
    public void renderGUI() {
        GUIEdit.editFloat3("Scale", scale, 0.1f);
        GUIEdit.editFloat3("Rotation", rotation, 0.1f);
        GUIEdit.editFloat3("Position", position, 0.1f);
    }
}
