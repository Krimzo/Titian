package script;

import entity.Entity;
import gui.GUIUtil;
import gui.abs.GUIRenderable;
import script.abs.Scriptable;
import utility.nncollection.NNArrayList;

import java.io.*;
import java.lang.reflect.*;
import java.util.Arrays;

public class Instance implements GUIRenderable, Serializable {
    private String name = null;
    private byte[] data = null;
    private Entity entity = null;

    private transient Scriptable instance = null;
    private transient Class<?> type = null;
    private transient NNArrayList<Field> fields = null;

    public Instance(String name, byte[] data, Entity entity) throws Exception {
        if (name == null || data == null || entity == null) {
            throw new Exception("Not enough data!");
        }

        this.name = name;
        this.data = Arrays.copyOf(data, data.length);
        this.entity = entity;

        loadType(name, data);
        loadInstance(entity);
        loadFields();
    }

    @Serial
    private void readObject(ObjectInputStream stream) throws IOException, ClassNotFoundException {
        stream.defaultReadObject();

        loadType(name, data);
        loadInstance(entity);
        loadFields();
    }

    public String getName() {
        return name;
    }

    private void loadType(String name, byte[] data) {
        try {
            type = new Loader().load(name, data);
        }
        catch (Exception e) {
            type = null;
            e.printStackTrace();
        }
    }

    private void loadInstance(Entity entity) {
        try {
            Constructor<?> constructor = type.getDeclaredConstructor(Entity.class);
            instance = (Scriptable) constructor.newInstance(entity);
        }
        catch (Exception e) {
            instance = null;
            e.printStackTrace();
        }
    }

    private void loadFields() {
        if (type == null || instance == null) {
            return;
        }

        fields = new NNArrayList<>();
        for (Field field : type.getDeclaredFields()) {
            if (field.canAccess(instance)) {
                fields.add(field);
            }
        }
    }

    public void callStarts() {
        if (instance == null) {
            return;
        }

        try {
            instance.start();
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void callUpdates() {
        if (instance == null) {
            return;
        }

        try {
            instance.update();
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Override
    public void renderGUI() {
        if (fields == null || instance == null) {
            return;
        }

        for (Field field : fields) {
            try {
                GUIUtil.editField(field, instance);
            }
            catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
}
