package entity;

import glparts.*;
import physics.*;
import renderer.*;

import java.io.Serializable;

public class Entity implements Physical, Renderable, Serializable {
    public final TransformComponent transformComponent = new TransformComponent();
    public final MeshComponent meshComponent = new MeshComponent();
    public final MaterialComponent materialComponent = new MaterialComponent();
    public final PhysicsComponent physicsComponent = new PhysicsComponent();

    public Entity() {}

    @Override
    public void onPhysicsUpdate(float deltaT) {
        physicsComponent.velocity = physicsComponent.velocity.add(physicsComponent.acceleration.mul(deltaT));
        transformComponent.position = transformComponent.position.add(physicsComponent.velocity.mul(deltaT));
        transformComponent.rotation = transformComponent.rotation.add(physicsComponent.angular.mul(deltaT));
    }

    @Override
    public void onRender(Shaders shaders) {
        if (shaders != null && materialComponent.isValid() && meshComponent.isValid()) {
            shaders.setUniform("W", transformComponent.matrix());
            materialComponent.bind();
            meshComponent.onRender(shaders);
        }
    }
}
