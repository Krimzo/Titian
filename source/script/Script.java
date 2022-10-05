package script;

import entity.Entity;
import named.Named;
import script.abs.Scriptable;
import utility.Files;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.Serial;
import java.io.Serializable;
import java.lang.reflect.Constructor;
import java.util.Arrays;

public class Script extends Named implements Serializable {
    private byte[] data;
    private String filepath = null;
    private Entity entity = null;
    private transient Scriptable instance = null;

    public Script(String name, byte[] data, Entity entity) {
        super(null, null);
        load(name, data, entity);
    }

    public Script(String filepath, Entity entity) {
        super(null, null);
        load(filepath, entity);
    }

    @Serial
    private void readObject(ObjectInputStream stream) throws IOException, ClassNotFoundException {
        stream.defaultReadObject();
        load(getName(), data, entity);
    }

    public void load(String name, byte[] data, Entity entity) {
        try {
            setName(name);
            if (data != this.data) {
                this.data = Arrays.copyOf(data, data.length);
            }
            this.entity = entity;

            Loader loader = new Loader();
            Class<?> loaded = loader.load(name, data);
            Constructor<?> constructor = loaded.getDeclaredConstructor(Entity.class);
            instance = (Scriptable) constructor.newInstance(entity);
        }
        catch (Exception e) {
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

    public boolean callStart() {
        try {
            instance.start();
            return true;
        }
        catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }

    public boolean callUpdate() {
        try {
            instance.update();
            return true;
        }
        catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }

    public static String getTemplate(String className) {
        return "import script.abs.Scriptable;\n" +
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
