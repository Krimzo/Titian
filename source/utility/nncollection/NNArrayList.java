package utility.nncollection;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Collection;

public class NNArrayList<T> extends ArrayList<T> implements Serializable {
    public NNArrayList() {}

    @Override
    public T set(int index, T object) {
        if (object != null) {
            return super.set(index, object);
        }
        return null;
    }

    @Override
    public boolean add(T object) {
        if (object != null) {
            return super.add(object);
        }
        return false;
    }

    @Override
    public void add(int index, T object) {
        if (object != null) {
            super.add(index, object);
        }
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

    @Override
    public boolean addAll(int index, Collection<? extends T> collection) {
        int counter = 0;
        for (T object : collection) {
            this.add(index + counter++, object);
        }
        return counter > 0;
    }
}
