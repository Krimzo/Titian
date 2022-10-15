package entity.component;

import entity.Entity;
import entity.abs.EntityComponent;
import gui.helper.GUIPopup;
import imgui.ImGui;
import script.Script;
import utility.nncollection.NNArrayList;

import java.io.Serializable;
import java.util.concurrent.atomic.AtomicInteger;

public class ScriptComponent extends EntityComponent implements Serializable {
    public final NNArrayList<Script> scripts = new NNArrayList<>();

    public ScriptComponent(Entity entity) {
        super(entity);
    }

    @Override
    public void free() {

    }

    public void reload() {
        for (Script script : scripts) {
            script.reload(entity);
        }
    }

    @Override
    public void renderGUI() {
        for (AtomicInteger i = new AtomicInteger(); i.get() < scripts.size(); i.getAndIncrement()) {
            ImGui.pushID(i.get());
            scripts.get(i.get()).renderGUI();
            ImGui.popID();

            GUIPopup.itemPopup("EditScripts" + i, () -> {
                if (ImGui.button("Delete")) {
                    scripts.remove(i.getAndDecrement());
                    GUIPopup.close();
                }
            });
        }
    }
}
