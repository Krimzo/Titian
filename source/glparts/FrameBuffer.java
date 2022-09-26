package glparts;

import math.Float4;
import math.Int2;
import window.GLContext;

import static org.lwjgl.opengl.GL33.*;

public class FrameBuffer extends GLObject implements Validated, Bindable {
    protected int buffer;
    protected Texture colorMap;
    protected DepthTexture depthMap;

    public FrameBuffer(GLContext context, Int2 size) {
        super(context);

        int[] buffer = new int[1];
        glGenFramebuffers(buffer);
        this.buffer = buffer[0];

        colorMap = new Texture(context, size, null);
        depthMap = new DepthTexture(context, size);
    }

    @Override
    public void destroy() {
        if (isValid()) {
            unbind();
            depthMap.destroy();
            colorMap.destroy();
            glDeleteFramebuffers(buffer);
            buffer = 0;
        }
    }

    @Override
    public boolean isValid() {
        return buffer != 0 && colorMap.isValid() && depthMap.isValid();
    }

    @Override
    public void bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, buffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorMap.texture, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap.texture, 0);
    }

    @Override
    public void unbind() {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    public void clear(boolean depth) {
        bind();
        glClear(GL_COLOR_BUFFER_BIT | (depth ? GL_DEPTH_BUFFER_BIT : 0));
    }

    public Texture getColorMap() {
        unbind();
        return colorMap;
    }

    public DepthTexture getDepthMap() {
        unbind();
        return depthMap;
    }

    public void updateSize(Int2 newSize) {
        getColorMap().updateSize(newSize);
        getDepthMap().updateSize(newSize);
    }

    public Float4 getPixel(Int2 pos) {
        return getPixels(pos, new Int2(1))[0];
    }

    public Float4[] getPixels(Int2 pos, Int2 size) {
        Float4[] result = new Float4[size.x * size.y];
        float[] data = new float[result.length * 4];

        bind();
        glReadPixels(pos.x, pos.y, size.x, size.y, GL_RGBA, GL_FLOAT, data);
        unbind();

        for (int i = 0; i < result.length; i++) {
            result[i] = new Float4(
                data[i * 4 + 0],
                data[i * 4 + 1],
                data[i * 4 + 2],
                data[i * 4 + 3]
            );
        }
        return result;
    }
}
