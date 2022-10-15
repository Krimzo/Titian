package glparts.abs;

import named.NameHolder;
import named.Named;
import utility.abs.Allocated;

import java.io.Serializable;

public abstract class GLObject extends Named implements Allocated, Serializable {
    public transient final GLContext context;

    public GLObject(NameHolder holder, String name, GLContext context) {
        super(holder, name);
        this.context = context;
    }
}
