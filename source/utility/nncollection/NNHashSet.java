package utility.nncollection;

import java.util.Collection;
import java.util.HashSet;

public class NNHashSet<T> extends HashSet<T> {
    public NNHashSet() {}

    @Override
    public boolean add(T object) {
        if (object != null) {
            return super.add(object);
        }
        return false;
    }

    @Override
    public boolean addAll(Collection<? extends T> collection) {
        boolean wasAdded = false;
        for (T object : collection) {
            if (this.add(object)) {
                wasAdded = true;
            }
        }
        return wasAdded;
    }
}
