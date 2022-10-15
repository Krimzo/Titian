package script;

import entity.Entity;
import gui.abs.GUIRenderable;
import gui.helper.GUIEdit;
import imgui.ImGui;
import script.abs.Scriptable;
import utility.Files;
import utility.Instance;
import utility.nncollection.NNArrayList;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.Serial;
import java.io.Serializable;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.util.Arrays;

public class Script implements GUIRenderable, Serializable {
    private String filepath = null;
    private String name = null;
    private byte[] data = null;
    private Entity entity = null;

    private transient Class<?> type = null;
    private transient Scriptable instance = null;
    private transient NNArrayList<Field> fields = null;

    public Script(String name, byte[] data, Entity entity) {
        load(name, data, entity);
    }

    public Script(String filepath, Entity entity) {
        load(filepath, entity);
    }

    @Serial
    private void readObject(ObjectInputStream stream) throws IOException, ClassNotFoundException {
        stream.defaultReadObject();

        loadType(name, data);
        loadInstance(entity);
        loadFields();
    }

    public void load(String name, byte[] data, Entity entity) {
        try {
            this.name = name;
            this.data = Instance.isValid(data) ? Arrays.copyOf(data, data.length) : null;
            this.entity = entity;

            loadType(name, data);
            loadInstance(entity);
            loadFields();
        }
        catch (Exception ignored) {
            System.out.println("Script \"" + name + "\" loading error");
        }
    }

    public void load(String filepath, Entity entity) {
        this.filepath = filepath;
        load(Files.getNameWithoutExtension(filepath), new Compiler().compile(filepath), entity);
    }

    public void reload(Entity entity) {
        load(filepath, entity);
    }

    private void loadType(String name, byte[] data) {
        try {
            type = new Loader().load(name, data);
        }
        catch (Exception ignored) {
            System.out.println("Script \"" + name + "\" type loading error");
            type = null;
        }
    }

    private void loadInstance(Entity entity) {
        try {
            Constructor<?> constructor = type.getDeclaredConstructor(Entity.class);
            instance = (Scriptable) constructor.newInstance(entity);
        }
        catch (Exception ignored) {
            System.out.println("Script \"" + name + "\" instance creation error");
            instance = null;
        }
    }

    private void loadFields() {
        try {
            fields = new NNArrayList<>();
            for (Field field : type.getDeclaredFields()) {
                if (field.canAccess(instance)) {
                    fields.add(field);
                }
            }
        }
        catch (Exception ignored) {
            System.out.println("Script \"" + name + "\" fields loading error");
            fields = null;
        }
    }

    public void callStarts() {
        try {
            instance.start();
        }
        catch (Exception ignored) {
            System.out.println("Script \"" + name + "\" start call error");
        }
    }

    public void callUpdates() {
        try {
            instance.update();
        }
        catch (Exception ignored) {
            System.out.println("Script \"" + name + "\" update call error");
        }
    }

    @Override
    public void renderGUI() {
        boolean valid = Instance.isValid(fields) && Instance.isValid(instance);

        if (ImGui.collapsingHeader(valid ? name : (name + "?")) && valid) {
            for (Field field : fields) {
                try {
                    GUIEdit.editField(field, instance);
                }
                catch (Exception ignored) {
                    System.out.println("Script \"" + name + "\" field gui error");
                }
            }
        }
    }

    public static String getTemplate(String className) {
        return "import script.abs.Scriptable;\n" +
            "import entity.Entity;\n" +
            "import window.input.*;\n"+
            "import java.awt.Color;\n" +
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
