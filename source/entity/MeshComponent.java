package entity;

import glparts.Mesh;
import glparts.Shaders;
import gui.GUIRenderable;
import renderer.Renderable;

import java.io.Serializable;

public class MeshComponent implements Renderable, GUIRenderable, Serializable {
    public Mesh mesh = null;

    public MeshComponent() {}

    public MeshComponent(Mesh mesh) {
        this.mesh = mesh;
    }

    @Override
    public void onRender(Shaders shaders) {
        mesh.onRender(shaders);
    }

    @Override
    public void onGUIRender() {

    }
}
