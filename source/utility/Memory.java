package utility;

import math.Vertex;
import org.lwjgl.BufferUtils;

import java.nio.ByteBuffer;

public final class Memory {
    private Memory() {}

    public static byte[] readByteBuffer(ByteBuffer buffer) {
        if (Instance.isValid(buffer)) {
            byte[] data = new byte[buffer.capacity()];

            buffer.get(data);
            buffer.rewind();

            return data;
        }
        return null;
    }

    public static ByteBuffer createByteBuffer(byte[] data) {
        if (Instance.isValid(data)) {
            ByteBuffer buffer = BufferUtils.createByteBuffer(data.length);

            buffer.put(data);
            buffer.rewind();

            return buffer;
        }
        return null;
    }

    public static float[] convertVertices(Vertex[] vertices) {
        float[] buffer = new float[vertices.length * 8];

        for(int i = 0; i < vertices.length; i++) {
            buffer[i * 8] = vertices[i].world.x;
            buffer[i * 8 + 1] = vertices[i].world.y;
            buffer[i * 8 + 2] = vertices[i].world.z;
            buffer[i * 8 + 3] = vertices[i].texture.x;
            buffer[i * 8 + 4] = vertices[i].texture.y;
            buffer[i * 8 + 5] = vertices[i].normal.x;
            buffer[i * 8 + 6] = vertices[i].normal.y;
            buffer[i * 8 + 7] = vertices[i].normal.z;
        }

        return buffer;
    }
}
