package entity.component;

import callback.EmptyCallback;
import entity.Entity;
import entity.abs.EntityComponent;
import imgui.ImGui;
import material.Material;

import java.io.Serializable;

public class MaterialComponent extends EntityComponent implements Serializable {
    public Material material = null;

    public MaterialComponent(Entity entity) {
        super(entity);
    }

    public MaterialComponent(Entity entity, Material material) {
        super(entity);
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
