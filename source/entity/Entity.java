package entity;

import entity.component.*;
import glparts.*;
import physics.Physical;
import renderer.abs.Renderable;
import named.*;
import script.Script;
import script.abs.Scriptable;

import java.io.Serializable;

public class Entity extends Named implements Physical, Scriptable, Renderable, Serializable {
    public final TransformComponent transformComponent = new TransformComponent();
    public final MeshComponent meshComponent = new MeshComponent();
    public final MaterialComponent materialComponent = new MaterialComponent();
    public final PhysicsComponent physicsComponent = new PhysicsComponent();
    public final ScriptComponent scriptComponent = new ScriptComponent();

    public Entity(NameHolder holder, String name) {
        super(holder, name);
    }

    @Override
    public void updatePhysics(float deltaT) {
        physicsComponent.velocity = physicsComponent.velocity.add(physicsComponent.acceleration.mul(deltaT));
        transformComponent.position = transformComponent.position.add(physicsComponent.velocity.mul(deltaT));
        transformComponent.rotation = transformComponent.rotation.add(physicsComponent.angular.mul(deltaT));
    }

    @Override
    public void scriptStart() {
        for (Script script : scriptComponent.scripts) {
            script.execute("scriptStart");
        }
    }

    @Override
    public void scriptUpdate() {
        for (Script script : scriptComponent.scripts) {
            script.execute("scriptUpdate");
        }
    }

    @Override
    public void render(Shaders shaders) {
        shaders.setUniform("W", transformComponent.matrix());
        materialComponent.use(() -> {
            meshComponent.render(shaders);
        });
    }
}
