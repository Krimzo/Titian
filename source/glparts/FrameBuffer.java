package glparts;

import math.Int2;
import window.GLContext;

import static org.lwjgl.opengl.GL33.*;

public class FrameBuffer extends GLObject {
    protected int buffer;
    protected Texture texture;

    public FrameBuffer(GLContext context, Int2 size) {
        super(context);

        int[] buffer = new int[1];
        glGenFramebuffers(buffer);
        this.buffer = buffer[0];

        texture = new Texture(context, size, null);
    }

    @Override
    public void destroy() {
        if (buffer != 0) {
            unbind();
            getTexture().destroy();
            glDeleteFramebuffers(buffer);
            buffer = 0;
        }
    }

    public void bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, buffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.buffer, 0);
    }

    public void unbind() {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    public void clear(boolean depth) {
        bind();
        glClear(GL_COLOR_BUFFER_BIT | (depth ? GL_DEPTH_BUFFER_BIT : 0));
    }

    public Texture getTexture() {
        unbind();
        return texture;
    }

    public void setSize(Int2 newSize) {
        getTexture().setSize(newSize);
    }

    public float[] getPixels(Int2 pos, Int2 size) {
        bind();
        float[] data = new float[size.x * size.y * 4];
        glReadPixels(pos.x, pos.y, size.x, size.y, GL_RGBA, GL_FLOAT, data);
        return data;
    }
}
