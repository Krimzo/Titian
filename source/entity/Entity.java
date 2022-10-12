package entity;

import editor.Editor;
import glparts.Shaders;
import imgui.ImGui;
import named.NameHolder;
import named.Named;
import physics.Physical;
import renderer.abs.Renderable;
import script.Script;

import java.io.Serializable;

public class Entity extends Named implements Physical, Renderable, Serializable {
    public final ComponentData components = new ComponentData(this);
    public transient Editor editor;

    public Entity(NameHolder holder, String name, Editor editor) {
        super(holder, name);
        this.editor = editor;
    }

    public void callScriptStarts() {
        for (Script script : components.script.scripts) {
            script.callStarts();
        }
    }

    public void callScriptUpdates() {
        for (Script script : components.script.scripts) {
            script.callUpdates();
        }
    }

    @Override
    public void updatePhysics(float deltaT) {
        components.physics.velocity.set(components.physics.velocity.add(components.physics.acceleration.multiply(deltaT)));
        components.transform.position.set(components.transform.position.add(components.physics.velocity.multiply(deltaT)));
        components.transform.rotation.set(components.transform.rotation.add(components.physics.angular.multiply(deltaT)));
    }

    @Override
    public void gameRender(Shaders shaders) {
        shaders.setUniform("W", components.transform.matrix());
        components.material.use(() -> {
            components.mesh.gameRender(shaders);
        });
    }

    @Override
    public void editorRender(Shaders shaders) {
        shaders.setUniform("W", components.transform.matrix());
        components.material.use(() -> {
            components.mesh.editorRender(shaders);
        });
    }

    @Override
    public void indexRender(Shaders shaders) {
        shaders.setUniform("W", components.transform.matrix());
        components.material.use(() -> {
            components.mesh.indexRender(shaders);
        });
    }

    public void renderInfoGUI(Editor editor) {
        ImGui.bulletText(getName());
    }
}
