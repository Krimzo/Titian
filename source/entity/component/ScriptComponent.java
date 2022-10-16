package entity.component;

import entity.Entity;
import entity.abs.EntityComponent;
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

    public void reload() {
        for (Script script : scripts) {
            script.reload(entity);
        }
    }

    @Override
    public void renderGUI() {
        for (AtomicInteger i = new AtomicInteger(); i.get() < scripts.size(); i.getAndIncrement()) {
            ImGui.pushID(i.get());
            scripts.get(i.get()).renderGUI(scripts, i);
            ImGui.popID();
        }
    }
}
