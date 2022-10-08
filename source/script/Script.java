package script;

import entity.Entity;
import gui.abs.GUIRenderable;
import imgui.ImGui;
import utility.Files;

import java.io.Serializable;

public class Script implements GUIRenderable, Serializable {
    private Instance instance = null;
    private String filepath = null;

    public Script(String name, byte[] data, Entity entity) {
        load(name, data, entity);
    }

    public Script(String filepath, Entity entity) {
        load(filepath, entity);
    }

    public void load(String name, byte[] data, Entity entity) {
        try {
            this.instance = new Instance(name, data, entity);
        }
        catch (Exception e) {
            this.instance = null;
            e.printStackTrace();
        }
    }

    public void load(String filepath, Entity entity) {
        this.filepath = filepath;
        load(Files.getNameWithoutExtension(filepath), new Compiler().compile(filepath), entity);
    }

    public void reload(Entity entity) {
        load(filepath, entity);
    }

    public void callStarts() {
        if (instance != null) {
            instance.callStarts();
        }
    }

    public void callUpdates() {
        if (instance != null) {
            instance.callUpdates();
        }
    }

    @Override
    public void renderGUI() {
        if (instance == null) {
            return;
        }

        if (ImGui.collapsingHeader(instance.getName())) {
            instance.renderGUI();
        }
    }

    public static String getTemplate(String className) {
        return "import script.abs.Scriptable;\n" +
            "import java.awt.Color;\n" +
            "import entity.Entity;\n" +
            "import math.*;\n" +
            "\n" +
            "public class " + className + " extends Scriptable {\n" +
            "    public " + className + "(Entity entity) {\n" +
            "        super(entity);\n" +
            "    }\n" +
            "\n" +
            "    @Override\n" +
            "    public void start() {\n" +
            "\n" +
            "    }\n" +
            "\n" +
            "    @Override\n" +
            "    public void update() {\n" +
            "\n" +
            "    }\n" +
            "}\n";
    }
}
