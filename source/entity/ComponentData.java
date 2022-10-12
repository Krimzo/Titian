package entity;

import entity.component.*;

import java.io.Serializable;

public final class ComponentData implements Serializable {
    public final TransformComponent transform;
    public final MeshComponent mesh;
    public final MaterialComponent material;
    public final PhysicsComponent physics;
    public final ScriptComponent script;

    public ComponentData(Entity entity) {
        transform = new TransformComponent(entity);
        mesh = new MeshComponent(entity);
        material = new MaterialComponent(entity);
        physics = new PhysicsComponent(entity);
        script = new ScriptComponent(entity);
    }
}
