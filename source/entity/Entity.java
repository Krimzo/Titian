package entity;

import editor.Editor;
import entity.component.*;
import glparts.*;
import imgui.ImGui;
import physics.Physical;
import named.*;
import renderer.abs.*;
import script.Script;

import java.io.Serializable;

public class Entity extends Named implements Physical, GameRenderable, EditorRenderable, IndexRenderable, Serializable {
    public transient final Editor editor;

    public final TransformComponent transformComponent = new TransformComponent(this);
    public final MeshComponent meshComponent = new MeshComponent(this);
    public final MaterialComponent materialComponent = new MaterialComponent(this);
    public final PhysicsComponent physicsComponent = new PhysicsComponent(this);
    public final ScriptComponent scriptComponent = new ScriptComponent(this);

    public Entity(NameHolder holder, String name, Editor editor) {
        super(holder, name);
        this.editor = editor;
    }

    public void callScriptStarts() {
        for (Script script : scriptComponent.scripts) {
            script.callStarts();
        }
    }

    public void callScriptUpdates() {
        for (Script script : scriptComponent.scripts) {
            script.callUpdates();
        }
    }

    @Override
    public void updatePhysics(float deltaT) {
        physicsComponent.velocity.set(physicsComponent.velocity.add(physicsComponent.acceleration.multiply(deltaT)));
        transformComponent.position.set(transformComponent.position.add(physicsComponent.velocity.multiply(deltaT)));
        transformComponent.rotation.set(transformComponent.rotation.add(physicsComponent.angular.multiply(deltaT)));
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

    public void renderInfoGUI(Editor editor) {
        ImGui.bulletText(getName());
    }
}
