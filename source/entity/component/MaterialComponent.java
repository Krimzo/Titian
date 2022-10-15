package entity.component;

import callback.EmptyCallback;
import entity.Entity;
import entity.abs.EntityComponent;
import gui.helper.GUIPopup;
import imgui.ImGui;
import material.Material;
import utility.Instance;

import java.io.Serializable;

public class MaterialComponent extends EntityComponent implements Serializable {
    public Material material = null;

    public MaterialComponent(Entity entity) {
        super(entity);
    }

    public void use(EmptyCallback callback) {
        if (Instance.isValid(material)) {
            material.use(callback);
        }
    }

    @Override
    public void renderGUI() {
        ImGui.bulletText(Instance.isValid(material) ? material.getName() : "None");

        GUIPopup.itemPopup("EditEntityMaterial", () -> {
            if (ImGui.button("Remove material")) {
                material = null;
                GUIPopup.close();
            }
        });
    }
}
