package glparts;

import window.GLContext;

public abstract class GLObject {
    public GLObject(GLContext context) {}
    public abstract void destroy();
}
