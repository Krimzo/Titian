package entity.component;

import entity.Entity;
import entity.abs.EntityComponent;
import imgui.ImGui;
import script.Script;
import utility.nncollection.NNArrayList;

import java.io.Serializable;

public class ScriptComponent extends EntityComponent implements Serializable {
    public final NNArrayList<Script> scripts = new NNArrayList<>();

    public ScriptComponent(Entity entity) {
        super(entity);
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

            if (ImGui.beginPopupContextItem("EditScripts" + i)) {
                if (ImGui.button("Remove")) {
                    scripts.remove(i--);
                    ImGui.closeCurrentPopup();
                }

                ImGui.endPopup();
            }
        }
    }
}
