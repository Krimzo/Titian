package glparts;

import window.GLContext;

import java.io.Serializable;

public abstract class GLObject implements Serializable {
    public GLObject(GLContext context) {}
    public abstract void destroy();
}
