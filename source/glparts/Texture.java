package glparts;

import callbacks.EmptyCallback;
import math.*;
import window.*;
import utility.File;
import utility.Memory;

import java.io.*;
import java.nio.ByteBuffer;
import java.util.*;

import static org.lwjgl.opengl.GL33.*;

public class Texture extends GLObject implements Serializable {
    private Int2 size;
    private byte[] pixels;
    private transient int buffer;

    public Texture(GLContext context, Int2 size, byte[] pixels) {
        super(context);

        this.size = new Int2(size);
        this.pixels = (pixels != null) ? Arrays.copyOf(pixels, pixels.length) : null;

        buffer = glGenTextures();

        glBindTexture(GL_TEXTURE_2D, buffer);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, Memory.createByteBuffer(pixels));

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    public Texture(GLContext context, String filepath) {
        this(context, File.getImageSize(filepath), File.getImageData(filepath));
    }

    @Override
    public void dispose() {
        if (buffer != 0) {
            glDeleteTextures(buffer);
            buffer = 0;
        }
    }

    @Serial
    private void writeObject(ObjectOutputStream stream) throws IOException {
        stream.defaultWriteObject();
        stream.writeObject(size);
        stream.writeObject(pixels);
    }

    @Serial
    private void readObject(ObjectInputStream stream) throws IOException, ClassNotFoundException {
        stream.defaultReadObject();
        Texture texture = new Texture(null, (Int2) stream.readObject(), (byte[]) stream.readObject());
        size = texture.size;
        pixels = texture.pixels;
        buffer = texture.buffer;
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

    public int getBuffer() {
        return buffer;
    }

    public Int2 getSize() {
        return new Int2(size);
    }

    public void resize(Int2 size) {
        this.size = new Int2(size);
        glBindTexture(GL_TEXTURE_2D, buffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, (ByteBuffer) null);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
