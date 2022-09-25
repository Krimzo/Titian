package entity;

import gui.GUIRenderable;
import material.Material;

import java.io.Serializable;

public class MaterialComponent implements Validated, GUIRenderable, Serializable {
    public Material material = new Material();

    public MaterialComponent() {}

    public MaterialComponent(Material material) {
        this.material = material;
    }

    public void bind() {
        if (isValid()) {
            material.bind();
        }
    }

    @Override
    public boolean isValid() {
        return material != null;
    }

    @Override
    public void onGUIRender() {

    }
}
