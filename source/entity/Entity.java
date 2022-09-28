package entity;

import glparts.*;
import interfaces.Disposable;
import interfaces.Physical;
import interfaces.Renderable;
import named.*;

import java.io.Serializable;

public class Entity extends Named implements Physical, Renderable, Disposable, Serializable {
    public final TransformComponent transformComponent = new TransformComponent();
    public final MeshComponent meshComponent = new MeshComponent();
    public final MaterialComponent materialComponent = new MaterialComponent();
    public final PhysicsComponent physicsComponent = new PhysicsComponent();

    public Entity(NameHolder holder) {
        super(holder);
    }

    @Override
    public void updatePhysics(float deltaT) {
        physicsComponent.velocity = physicsComponent.velocity.add(physicsComponent.acceleration.mul(deltaT));
        transformComponent.position = transformComponent.position.add(physicsComponent.velocity.mul(deltaT));
        transformComponent.rotation = transformComponent.rotation.add(physicsComponent.angular.mul(deltaT));
    }

    @Override
    public void render(Shaders shaders) {
        shaders.setUniform("W", transformComponent.matrix());
        materialComponent.use(() -> {
            meshComponent.render(shaders);
        });
    }

    @Override
    public void dispose() {
        meshComponent.dispose();
        materialComponent.dispose();
    }
}
