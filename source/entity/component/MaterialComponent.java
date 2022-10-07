package entity.component;

import callback.EmptyCallback;
import entity.Entity;
import gui.abs.GUIRenderable;
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
        ImGui.bulletText((material != null) ? material.getName() : "None");

        if (ImGui.beginPopupContextItem("EditEntityMaterial")) {
            if (ImGui.button("Remove material")) {
                material = null;

                ImGui.closeCurrentPopup();
            }

            ImGui.endPopup();
        }
    }
}
