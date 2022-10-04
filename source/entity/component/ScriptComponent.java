package entity.component;

import gui.abs.GUIRenderable;
import imgui.ImGui;
import script.Script;

import java.io.Serializable;
import java.util.ArrayList;

public class ScriptComponent implements GUIRenderable, Serializable {
    public final ArrayList<Script> scripts = new ArrayList<>();

    public ScriptComponent() {}

    @Override
    public void renderGUI() {
        for (Script script : scripts) {
            ImGui.text(script.getName());
        }
    }
}
