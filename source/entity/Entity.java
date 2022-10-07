package entity;

import editor.Editor;
import entity.component.*;
import glparts.*;
import physics.Physical;
import named.*;
import renderer.abs.EditorRenderable;
import renderer.abs.GameRenderable;
import renderer.abs.IndexRenderable;
import script.Script;

import java.io.Serializable;

public class Entity extends Named implements Physical, GameRenderable, EditorRenderable, IndexRenderable, Serializable {
    public transient final Editor editor;

    public final TransformComponent transformComponent = new TransformComponent();
    public final MeshComponent meshComponent = new MeshComponent();
    public final MaterialComponent materialComponent = new MaterialComponent();
    public final PhysicsComponent physicsComponent = new PhysicsComponent();
    public final ScriptComponent scriptComponent = new ScriptComponent(this);

    public Entity(NameHolder holder, String name, Editor editor) {
        super(holder, name);
        this.editor = editor;
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
                physicsComponent.velocity = physicsComponent.velocity.add(physicsComponent.acceleration.multiply(deltaT));
            }
            if (transformComponent.position != null) {
                transformComponent.position = transformComponent.position.add(physicsComponent.velocity.multiply(deltaT));
            }
        }

        if (transformComponent.rotation != null && physicsComponent.angular != null) {
            transformComponent.rotation = transformComponent.rotation.add(physicsComponent.angular.multiply(deltaT));
        }
    }

    @Override
    public void gameRender(Shaders shaders) {
        shaders.setUniform("W", transformComponent.matrix());
        materialComponent.use(() -> {
            meshComponent.gameRender(shaders);
        });
    }

    @Override
    public void editorRender(Shaders shaders) {
        shaders.setUniform("W", transformComponent.matrix());
        materialComponent.use(() -> {
            meshComponent.editorRender(shaders);
        });
    }

    @Override
    public void indexRender(Shaders shaders) {
        shaders.setUniform("W", transformComponent.matrix());
        materialComponent.use(() -> {
            meshComponent.indexRender(shaders);
        });
    }

    public void renderInfoGUI(Editor editor) {}
}
