package entity;

import glparts.Mesh;
import glparts.Shaders;
import gui.GUIRenderable;
import renderer.Renderable;

import java.io.Serializable;

public class MeshComponent implements Validated, Renderable, GUIRenderable, Serializable {
    public Mesh mesh = null;

    public MeshComponent() {}

    public MeshComponent(Mesh mesh) {
        this.mesh = mesh;
    }

    @Override
    public boolean isValid() {
        return mesh != null;
    }

    @Override
    public void onRender(Shaders shaders) {
        if (isValid()) {
            mesh.onRender(shaders);
        }
    }

    @Override
    public void onGUIRender() {

    }
}
