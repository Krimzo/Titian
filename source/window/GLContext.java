package window;

import math.Float4;
import math.Int2;
import org.lwjgl.opengl.GL;

import static org.lwjgl.opengl.GL33.*;

public final class GLContext {
    public GLContext() {
        GL.createCapabilities();
    }

    public void destroy() {
        GL.setCapabilities(null);
    }

    public void setViewport(Int2 size) {
        setViewport(new Int2(), size);
    }

    public void setViewport(Int2 pos, Int2 size) {
        glViewport(pos.x, pos.y, size.x, size.y);
    }

    public void setClearColor(Float4 color) {
        glClearColor(color.x, color.y, color.z, color.w);
    }

    public void clear(boolean depth) {
        glClear(GL_COLOR_BUFFER_BIT | (depth ? GL_DEPTH_BUFFER_BIT : 0));
    }

    public void setDepthTest(boolean enabled) {
        if (enabled) {
            glEnable(GL_DEPTH_TEST);
        }
        else {
            glDisable(GL_DEPTH_TEST);
        }
    }

    public void setWireframe(boolean wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
    }

    public void setCull(boolean enabled, boolean cullBack) {
        if (enabled) {
            glEnable(GL_CULL_FACE);
            glCullFace(cullBack ? GL_BACK : GL_FRONT);
        }
        else {
            glDisable(GL_CULL_FACE);
        }
    }
}
