package entity;

import entity.component.*;

import java.io.Serializable;

public final class ComponentPackage implements Serializable {
    public final TransformComponent transform;
    public final MeshComponent mesh;
    public final MaterialComponent material;
    public final PhysicsComponent physics;
    public final ScriptComponent script;

    public ComponentPackage(Entity entity) {
        transform = new TransformComponent(entity);
        mesh = new MeshComponent(entity);
        material = new MaterialComponent(entity);
        physics = new PhysicsComponent(entity);
        script = new ScriptComponent(entity);
    }
}
