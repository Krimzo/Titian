package glparts;

import window.GLContext;

import java.io.Serializable;

public abstract class GLObject implements Serializable {
    public transient final GLContext context;

    public GLObject(GLContext context) {
        this.context = context;
    }

    public abstract void destroy();
}
