package entity;

import callbacks.EmptyCallback;
import interfaces.Disposable;
import interfaces.GUIRenderable;
import material.Material;

import java.io.Serializable;

public class MaterialComponent implements GUIRenderable, Disposable, Serializable {
    public Material material = new Material();

    public MaterialComponent() {}

    public MaterialComponent(Material material) {
        this.material = material;
    }

    public void use(EmptyCallback callback) {
        if (material != null) {
            material.use(callback);
        }
        else {
            callback.method();
        }
    }

    @Override
    public void renderGUI() {

    }

    @Override
    public void dispose() {
        if (material != null) {
            material.dispose();
            material = null;
        }
    }
}
