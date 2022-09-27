package glparts;

import editor.Editor;
import math.*;
import org.lwjgl.BufferUtils;
import window.*;

import java.io.*;
import java.nio.ByteBuffer;
import java.util.Arrays;

import static org.lwjgl.opengl.GL33.*;
import org.lwjgl.stb.*;

public class Texture extends GLObject implements Validated, Bindable, Serializable {
    protected Int2 size = null;
    protected byte[] pixels = null;
    protected transient int texture = 0;

    public Texture(GLContext context, Int2 size, byte[] pixels) {
        super(context);
        loadData(size, pixels);
    }

    public Texture(GLContext context, String filepath) {
        this(context, getImageSize(filepath), getImageData(filepath));
    }

    public void loadData(Int2 size, byte[] data) {
        if (size != this.size) {
            this.size = new Int2(size);
        }
        if (data != this.pixels) {
            this.pixels = Arrays.copyOf(data, data.length);
        }

        destroy();
        texture = glGenTextures();

        bind();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, createByteBuffer(data));
        glGenerateMipmap(GL_TEXTURE_2D);

        setWrap(new Int2(GL_REPEAT));
        setFilters(new Int2(GL_LINEAR));
    }

    @Override
    public void destroy() {
        if (isValid()) {
            unbind();
            glDeleteTextures(texture);
            texture = 0;
        }
    }

    @Serial
    private void writeObject(ObjectOutputStream stream) throws IOException {
        stream.defaultWriteObject();
        stream.writeObject(size);
        stream.writeObject(pixels);

        if (Editor.DEBUG) {
            System.out.println("Texture saved!");
        }
    }

    @Serial
    private void readObject(ObjectInputStream stream) throws IOException, ClassNotFoundException {
        stream.defaultReadObject();
        size = (Int2) stream.readObject();
        pixels = (byte[]) stream.readObject();
        loadData(size, pixels);

        if (Editor.DEBUG) {
            System.out.println("Texture loaded!");
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

    public void bind(int slot) {
        glActiveTexture(GL_TEXTURE0 + slot);
        bind();
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
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, newSize.x, newSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, (ByteBuffer) null);
            size = new Int2(newSize);
        }
    }

    public byte[] getPixels() {
        return pixels;
    }

    public void setWrap(Int2 modes) {
        bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, modes.x);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, modes.y);
    }

    public void setFilters(Int2 filters) {
        bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filters.x);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filters.y);
    }

    public static Int2 getImageSize(String filePath) {
        int[] width = new int[1]; int[] height = new int[1]; int[] dontCare = new int[1];
        STBImage.stbi_info(filePath, width, height, dontCare);
        return new Int2(width[0], height[0]);
    }

    public static byte[] getImageData(String filepath) {
        return getImageData(filepath, true);
    }

    public static byte[] getImageData(String filepath, boolean flipOnY) {
        int[] dontCare = new int[1];
        STBImage.stbi_set_flip_vertically_on_load(flipOnY);
        return readByteBuffer(STBImage.stbi_load(filepath, dontCare, dontCare, dontCare, 4));
    }

    public static byte[] readByteBuffer(ByteBuffer buffer) {
        if (buffer != null) {
            byte[] data = new byte[buffer.capacity()];
            buffer.get(data);
            return data;
        }
        return null;
    }

    public static ByteBuffer createByteBuffer(byte[] data) {
        if (data != null) {
            ByteBuffer buffer = BufferUtils.createByteBuffer(data.length);
            buffer.put(data);
            buffer.rewind();
            return buffer;
        }
        return null;
    }
}
