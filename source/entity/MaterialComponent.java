package entity;

import gui.GUIRenderable;
import material.Material;

import java.io.Serializable;

public class MaterialComponent implements GUIRenderable, Serializable {
    public Material material = new Material();

    public MaterialComponent() {}

    public MaterialComponent(Material material) {
        this.material = material;
    }

    public void bind() {
        material.bind();
    }

    @Override
    public void onGUIRender() {

    }
}
