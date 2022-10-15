package utility;

public final class Instance {
    private Instance() {}

    public static boolean isNull(Object obj) {
        return obj == null;
    }

    public static boolean isValid(Object obj) {
        return !isNull(obj);
    }
}
