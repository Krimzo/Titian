package script;

public class Loader extends ClassLoader {
    public Loader() {}

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
