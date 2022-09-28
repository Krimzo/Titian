package entity;

import interfaces.Disposable;
import glparts.Mesh;
import glparts.Shaders;
import interfaces.GUIRenderable;
import interfaces.Renderable;

import java.io.Serializable;

public class MeshComponent implements Renderable, GUIRenderable, Disposable, Serializable {
    public Mesh mesh = null;

    public MeshComponent() {}

    public MeshComponent(Mesh mesh) {
        this.mesh = mesh;
    }

    @Override
    public void render(Shaders shaders) {
        if (mesh != null) {
            mesh.render(shaders);
        }
    }

    @Override
    public void renderGUI() {

    }

    @Override
    public void dispose() {
        if (mesh != null) {
            mesh.dispose();
            mesh = null;
        }
    }
}
