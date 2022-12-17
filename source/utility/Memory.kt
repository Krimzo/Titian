package utility

import math.Vertex
import org.lwjgl.BufferUtils
import java.nio.ByteBuffer

object Memory {
    fun readByteBuffer(buffer: ByteBuffer): ByteArray {
        val data = ByteArray(buffer.capacity())
        buffer[data]
        buffer.rewind()
        return data
    }

    fun createByteBuffer(data: ByteArray): ByteBuffer {
        val buffer = BufferUtils.createByteBuffer(data.size)
        buffer.put(data)
        buffer.rewind()
        return buffer
    }

    fun convertVertices(vertices: Array<Vertex>): FloatArray {
        val buffer = FloatArray(vertices.size * 8)
        for (i in vertices.indices) {
            buffer[i * 8 + 0] = vertices[i].world.x
            buffer[i * 8 + 1] = vertices[i].world.y
            buffer[i * 8 + 2] = vertices[i].world.z
            buffer[i * 8 + 3] = vertices[i].texture.x
            buffer[i * 8 + 4] = vertices[i].texture.y
            buffer[i * 8 + 5] = vertices[i].normal.x
            buffer[i * 8 + 6] = vertices[i].normal.y
            buffer[i * 8 + 7] = vertices[i].normal.z
        }
        return buffer
    }
}