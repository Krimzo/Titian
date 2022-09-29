package glparts;

import callback.EmptyCallback;
import math.Float4;
import math.Int2;
import window.GLContext;

import static org.lwjgl.opengl.GL33.*;

public class FrameBuffer extends GLObject {
    private Texture colorMap;
    private DepthTexture depthMap;
    private int buffer;

    public FrameBuffer(GLContext context, Int2 size) {
        super(null, null, context);

        colorMap = new Texture(null, null, context, size, null);
        depthMap = new DepthTexture(context, size);
        buffer = glGenFramebuffers();

        glBindFramebuffer(GL_FRAMEBUFFER, buffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorMap.getBuffer(), 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap.getBuffer(), 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    @Override
    public void dispose() {
        if (buffer != 0) {
            glDeleteFramebuffers(buffer);
            colorMap.dispose();
            depthMap.dispose();

            buffer = 0;
            colorMap = null;
            depthMap = null;
        }
    }

    public void clear() {
        glBindFramebuffer(GL_FRAMEBUFFER, buffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    public void use(EmptyCallback callback) {
        glBindFramebuffer(GL_FRAMEBUFFER, buffer);
        callback.method();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    public Texture getColorMap() {
        return colorMap;
    }

    public DepthTexture getDepthMap() {
        return depthMap;
    }

    public void resize(Int2 size) {
        if (size.x > 0 && size.y > 0) {
            colorMap.resize(size);
            depthMap.resize(size);
        }
    }

    public Float4 getPixel(Int2 pos) {
        return getPixels(pos, new Int2(1))[0];
    }

    public Float4[] getPixels(Int2 pos, Int2 size) {
        Float4[] result = new Float4[size.x * size.y];
        float[] data = new float[result.length * 4];

        glBindFramebuffer(GL_FRAMEBUFFER, buffer);
        glReadPixels(pos.x, colorMap.getSize().y - 1 - pos.y, size.x, size.y, GL_RGBA, GL_FLOAT, data);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        for (int i = 0; i < result.length; i++) {
            result[i] = new Float4(
                data[i * 4],
                data[i * 4 + 1],
                data[i * 4 + 2],
                data[i * 4 + 3]
            );
        }

        return result;
    }
}
