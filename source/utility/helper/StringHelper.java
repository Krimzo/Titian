package utility.helper;

import utility.nncollection.NNHashMap;

import java.util.Map;

public final class StringHelper {
    private StringHelper() {}

    private static final Map<Integer, String> spaces = new NNHashMap<>();

    public static String spaces(int count) {
        if (count < 1) {
            return "";
        }

        if (spaces.containsKey(count)) {
            return spaces.get(count);
        }

        return spaces.put(count, " ".repeat(count));
    }
}
