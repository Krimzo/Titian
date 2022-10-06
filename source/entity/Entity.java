package entity;

import editor.Editor;
import entity.component.*;
import glparts.*;
import physics.Physical;
import renderer.abs.Renderable;
import named.*;
import script.Script;

import java.io.Serializable;

public class Entity extends Named implements Physical, Renderable, Serializable {
    public final TransformComponent transformComponent = new TransformComponent();
    public final MeshComponent meshComponent = new MeshComponent();
    public final MaterialComponent materialComponent = new MaterialComponent();
    public final PhysicsComponent physicsComponent = new PhysicsComponent();
    public final ScriptComponent scriptComponent = new ScriptComponent(this);

    public Entity(NameHolder holder, String name) {
        super(holder, name);
    }

    public void callScriptStarts() {
        for (Script script : scriptComponent.scripts) {
            script.callStart();
        }
    }

    public void callScriptUpdates() {
        for (Script script : scriptComponent.scripts) {
            script.callUpdate();
        }
    }

    @Override
    public void updatePhysics(float deltaT) {
        if (physicsComponent.velocity != null) {
            if (physicsComponent.acceleration != null) {
                physicsComponent.velocity = physicsComponent.velocity.add(physicsComponent.acceleration.mul(deltaT));
            }
            if (transformComponent.position != null) {
                transformComponent.position = transformComponent.position.add(physicsComponent.velocity.mul(deltaT));
            }
        }

        if (transformComponent.rotation != null && physicsComponent.angular != null) {
            transformComponent.rotation = transformComponent.rotation.add(physicsComponent.angular.mul(deltaT));
        }
    }

    @Override
    public void render(Shaders shaders) {
        shaders.setUniform("W", transformComponent.matrix());
        materialComponent.use(() -> {
            meshComponent.render(shaders);
        });
    }

    public void renderCustomGUI(Editor editor) {}
}
