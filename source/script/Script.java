package script;

import named.Named;
import utility.Files;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.Serial;
import java.io.Serializable;
import java.lang.reflect.Method;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;

public class Script extends Named implements Serializable {
    private byte[] data;
    private transient Loader loader = null;
    private transient Object instance = null;
    private transient Map<String, Method> methods = null;

    public Script(String name, byte[] data) {
        super(null, null);
        reload(name, data);
    }

    public Script(String filepath) {
        super(null, null);
        reload(filepath);
    }

    @Serial
    private void readObject(ObjectInputStream stream) throws IOException, ClassNotFoundException {
        stream.defaultReadObject();
        reload(getName(), data);
    }

    public void reload(String name, byte[] data) {
        try {
            setName(name);
            if (data != this.data) {
                this.data = Arrays.copyOf(data, data.length);
            }

            loader = new Loader();
            Class<?> loadedClass = loader.load(name, data);

            instance = loadedClass.newInstance();
            methods = new HashMap<>();

            for (Method method : loadedClass.getDeclaredMethods()) {
                methods.put(method.getName(), method);
            }
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void reload(String filepath) {
        reload(Files.getNameWithoutExtension(filepath), new Compiler().compile(filepath));
    }

    public boolean execute(String method) {
        try {
            methods.get(method).invoke(instance);
            return true;
        }
        catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }
}
