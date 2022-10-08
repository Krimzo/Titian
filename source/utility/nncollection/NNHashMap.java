package utility.nncollection;

import java.util.HashMap;
import java.util.Map;

public class NNHashMap<K, V> extends HashMap<K, V> {
    public NNHashMap() {}

    @Override
    public V put(K key, V value) {
        if (key != null && value != null) {
            return super.put(key, value);
        }
        return null;
    }

    @Override
    public void putAll(Map<? extends K, ? extends V> map) {
        for (var pair : map.entrySet()) {
            this.put(pair.getKey(), pair.getValue());
        }
    }

    @Override
    public V replace(K key, V value) {
        if (key != null && value != null) {
            return super.replace(key, value);
        }
        return null;
    }
}
