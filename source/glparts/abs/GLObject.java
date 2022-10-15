package glparts.abs;

import named.NameHolder;
import named.Named;
import utility.nncollection.NNHashSet;
import window.GLContext;

import java.io.Serializable;
import java.util.HashSet;

public abstract class GLObject extends Named implements Serializable {
    private static final HashSet<GLObject> objects = new NNHashSet<>();
    private static final Object lock = new Object();

    public transient final GLContext context;

    public GLObject(NameHolder holder, String name, GLContext context) {
        super(holder, name);
        this.context = context;
    }

    @Override
    protected final void finalize() {
        if (isAllocated()) {
            synchronized (lock) {
                objects.add(this);
            }
        }
    }

    public abstract void deallocate();

    public abstract boolean isAllocated();

    public static void cleanup() {
        synchronized (lock) {
            for (GLObject object : objects) {
                object.deallocate();
            }
            objects.clear();
        }
    }
}
