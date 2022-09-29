package entity.components;

import callback.EmptyCallback;
import gui.GUIRenderable;
import imgui.ImGui;
import material.Material;

import java.io.Serializable;

public class MaterialComponent implements GUIRenderable, Serializable {
    public Material material = null;

    public MaterialComponent() {}

    public MaterialComponent(Material material) {
        this.material = material;
    }

    public void use(EmptyCallback callback) {
        if (material != null) {
            material.use(callback);
        }
    }

    @Override
    public void renderGUI() {
        ImGui.text((material != null) ? material.getName() : "NULL");
    }
}
