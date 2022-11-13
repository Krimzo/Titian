package script;

public class ScriptLoader extends ClassLoader {
    public ScriptLoader() {}

    public Class<?> load(String name, byte[] data) {
        defineClass(name, data, 0, data.length);

        try {
            return loadClass(name);
        }
        catch (Exception ignored) {
            return null;
        }
    }
}
