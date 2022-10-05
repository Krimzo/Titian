package entity.component;

import entity.Entity;
import gui.abs.GUIRenderable;
import imgui.ImGui;
import script.Script;

import java.io.Serializable;
import java.util.ArrayList;

public class ScriptComponent implements GUIRenderable, Serializable {
    private Entity entity = null;
    public final ArrayList<Script> scripts = new ArrayList<>();

    public ScriptComponent(Entity entity) {
        this.entity = entity;
    }

    public void reload() {
        for (Script script : scripts) {
            script.reload(entity);
        }
    }

    @Override
    public void renderGUI() {
        for (int i = 0; i < scripts.size(); i++) {
            ImGui.selectable(scripts.get(i).getName());

            if (ImGui.beginPopupContextItem()) {
                if (ImGui.button("Remove")) {
                    scripts.remove(i--);
                    ImGui.closeCurrentPopup();
                }

                ImGui.endPopup();
            }
        }
    }
}
