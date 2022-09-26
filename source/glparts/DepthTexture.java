package glparts;

import math.Int2;
import window.GLContext;

import java.nio.ByteBuffer;

import static org.lwjgl.opengl.GL33.*;

public class DepthTexture extends GLObject implements Validated, Bindable {
    protected Int2 size = null;
    protected int texture = 0;

    public DepthTexture(GLContext context, Int2 size) {
        super(context);

        this.size = new Int2(size);
        texture = glGenTextures();

        bind();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, size.x, size.y, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, (ByteBuffer) null);
        unbind();
    }

    @Override
    public void destroy() {
        if (isValid()) {
            unbind();
            glDeleteTextures(texture);
            texture = 0;
        }
    }

    @Override
    public boolean isValid() {
        return texture != 0;
    }

    @Override
    public void bind() {
        glBindTexture(GL_TEXTURE_2D, texture);
    }

    @Override
    public void unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    public int getID() {
        return texture;
    }

    public Int2 getSize() {
        return new Int2(size);
    }

    public void updateSize(Int2 newSize) {
        if (isValid() && !newSize.equals(size)) {
            bind();
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, size.x, size.y, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, (ByteBuffer) null);
            size = new Int2(newSize);
            unbind();
        }
    }
}
