package glparts;

import math.*;
import window.*;

import java.nio.ByteBuffer;
import java.util.Arrays;

import static org.lwjgl.opengl.GL33.*;
import org.lwjgl.stb.*;

public class Texture extends GLObject {
    protected Int2 size;
    protected byte[] data;
    protected int buffer;

    public Texture(GLContext context, Int2 size, ByteBuffer data) {
        super(context);

        byte[] bufferData = data.array();
        this.size = new Int2(size);
        this.data = Arrays.copyOf(bufferData, bufferData.length);

        buffer = glGenTextures();
        if (buffer == 0) {
            throw new Error("Could not create texture buffer!");
        }

        setWrap(new Int2(GL_REPEAT));
        setFilters(new Int2(GL_LINEAR));
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    public Texture(GLContext context, String filepath) {
        this(context, getImageSize(filepath), getImageData(filepath));
    }

    @Override
    public void destroy() {
        if (buffer != 0) {
            glDeleteTextures(buffer);
            buffer = 0;
        }
    }

    public int getID() {
        return buffer;
    }

    public Int2 getSize() {
        return size;
    }

    public void setSize(Int2 newSize) {
        if (buffer > 0) {
            glBindTexture(GL_TEXTURE_2D, buffer);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, newSize.x, newSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, (ByteBuffer) null);
            size = newSize;
        }
    }

    public static Int2 getImageSize(String filePath) {
        int[] width = new int[1]; int[] height = new int[1]; int[] dontCare = new int[1];
        STBImage.stbi_info(filePath, width, height, dontCare);
        return new Int2(width[0], height[0]);
    }

    public static ByteBuffer getImageData(String filePath) {
        int[] dontCare = new int[1];
        STBImage.stbi_set_flip_vertically_on_load(true);
        return STBImage.stbi_load(filePath, dontCare, dontCare, dontCare, 4);
    }

    public void setWrap(Int2 modes) {
        glBindTexture(GL_TEXTURE_2D, buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, modes.x);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, modes.y);
    }

    public void setFilters(Int2 filters) {
        glBindTexture(GL_TEXTURE_2D, buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filters.x);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filters.y);
    }

    public void bind(int slot) {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, buffer);
    }

    public static void unbind(int slot) {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
