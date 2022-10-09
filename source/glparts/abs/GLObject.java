package glparts.abs;

import named.NameHolder;
import named.Named;

import java.io.Serializable;

public abstract class GLObject extends Named implements Disposable, Serializable {
    public transient final GLContext context;

    public GLObject(NameHolder holder, String name, GLContext context) {
        super(holder, name);
        this.context = context;
    }
}
