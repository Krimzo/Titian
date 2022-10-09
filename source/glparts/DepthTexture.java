package glparts;

import callback.EmptyCallback;
import glparts.abs.GLContext;
import glparts.abs.GLObject;
import math.Int2;

import java.nio.ByteBuffer;

import static org.lwjgl.opengl.GL33.*;

public class DepthTexture extends GLObject {
    private Int2 size;
    private transient int buffer;

    public DepthTexture(GLContext context, Int2 size) {
        super(null, null, context);

        this.size = new Int2(size);

        buffer = glGenTextures();

        glBindTexture(GL_TEXTURE_2D, buffer);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size.x, size.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, (ByteBuffer) null);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    @Override
    public void dispose() {
        if (buffer != 0) {
            glDeleteTextures(buffer);
            buffer = 0;
        }
    }

    public void use(EmptyCallback callback) {
        glBindTexture(GL_TEXTURE_2D, buffer);
        callback.method();
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    public void use(int slot, EmptyCallback callback) {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, buffer);

        callback.method();

        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, 0);

        glActiveTexture(GL_TEXTURE0);
    }

    public int getBuffer() {
        return buffer;
    }

    public Int2 getSize() {
        return new Int2(size);
    }

    public void resize(Int2 size) {
        if (!this.size.equals(size)) {
            glBindTexture(GL_TEXTURE_2D, buffer);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size.x, size.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, (ByteBuffer) null);
            glBindTexture(GL_TEXTURE_2D, 0);
            this.size = new Int2(size);
        }
    }
}
