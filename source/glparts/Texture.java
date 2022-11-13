package glparts;

import callback.EmptyCallback;
import glparts.abs.GLObject;
import math.Int2;
import named.NameHolder;
import utility.Instance;
import utility.Memory;
import utility.helper.FileHelper;
import window.GLContext;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.Serial;
import java.io.Serializable;
import java.nio.ByteBuffer;
import java.util.Arrays;

import static org.lwjgl.opengl.GL33.*;

public class Texture extends GLObject implements Serializable {
    private final Int2 size;
    private final byte[] pixels;
    private transient int buffer;

    public Texture(NameHolder holder, String name, GLContext context, Int2 size, byte[] pixels) {
        super(holder, name, context);

        this.size = new Int2(size);
        this.pixels = Instance.isValid(pixels) ? Arrays.copyOf(pixels, pixels.length) : null;

        buffer = generateTexture(size, pixels);
    }

    public Texture(NameHolder holder, String name, GLContext context, String filepath, boolean flipY) {
        this(holder, name, context, FileHelper.getImageSize(filepath), FileHelper.getImageData(filepath, flipY));
    }

    public Texture(NameHolder holder, String name, GLContext context, String filepath) {
        this(holder, name, context, filepath, true);
    }

    @Override
    public void deallocate() {
        glDeleteTextures(buffer);
        buffer = 0;
    }

    @Override
    public boolean isAllocated() {
        return buffer != 0;
    }

    @Serial
    private void readObject(ObjectInputStream stream) throws IOException, ClassNotFoundException {
        stream.defaultReadObject();
        buffer = generateTexture(size, pixels);
    }

    public void setWrap(int wrapS, int wrapT) {
        glBindTexture(GL_TEXTURE_2D, buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    public void setFilters(int min, int mag) {
        glBindTexture(GL_TEXTURE_2D, buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
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
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, (ByteBuffer) null);
            glBindTexture(GL_TEXTURE_2D, 0);
            this.size.set(size);
        }
    }

    public static int generateTexture(Int2 size, byte[] pixels) {
        int buffer = glGenTextures();

        glBindTexture(GL_TEXTURE_2D, buffer);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, Memory.createByteBuffer(pixels));

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glBindTexture(GL_TEXTURE_2D, 0);

        return buffer;
    }

    public static void use(int[] textures, EmptyCallback callback) {
        for (int i = 0; i < textures.length; i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, textures[i]);
        }

        callback.method();

        for (int i = 0; i < textures.length; i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        glActiveTexture(GL_TEXTURE0);
    }
}
