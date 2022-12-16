package glparts

import glparts.abs.GLObject
import math.Int2
import named.NameHolder
import org.lwjgl.opengl.GL11
import org.lwjgl.opengl.GL13
import org.lwjgl.opengl.GL30
import utility.Memory
import utility.helper.FileHelper
import window.GLContext
import java.io.IOException
import java.io.ObjectInputStream
import java.io.Serial
import java.io.Serializable
import java.nio.ByteBuffer

class Texture(holder: NameHolder?, name: String?, context: GLContext?, size: Int2, pixels: ByteArray?) : GLObject(holder, name, context), Serializable {
    private val size: Int2
    private val pixels: ByteArray?

    @Transient
    var buffer: Int
        private set

    init {
        this.size = Int2(size)
        this.pixels = pixels?.copyOf()
        buffer = generateTexture(size, pixels)
    }

    constructor(holder: NameHolder?, name: String?, context: GLContext?, filepath: String, flipY: Boolean = true) : this(holder, name, context, FileHelper.getImageSize(filepath), FileHelper.getImageData(filepath, flipY)!!)

    override fun deallocate() {
        GL11.glDeleteTextures(buffer)
        buffer = 0
    }

    override fun isAllocated(): Boolean {
        return buffer != 0
    }

    @Serial
    @Throws(IOException::class, ClassNotFoundException::class)
    private fun readObject(stream: ObjectInputStream) {
        stream.defaultReadObject()
        buffer = generateTexture(size, pixels)
    }

    fun setWrap(wrapS: Int, wrapT: Int) {
        GL11.glBindTexture(GL11.GL_TEXTURE_2D, buffer)
        GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_WRAP_S, wrapS)
        GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_WRAP_T, wrapT)
        GL11.glBindTexture(GL11.GL_TEXTURE_2D, 0)
    }

    fun setFilters(min: Int, mag: Int) {
        GL11.glBindTexture(GL11.GL_TEXTURE_2D, buffer)
        GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_MIN_FILTER, min)
        GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_MAG_FILTER, mag)
        GL11.glBindTexture(GL11.GL_TEXTURE_2D, 0)
    }

    fun use(slot: Int, callback: () -> Unit) {
        GL13.glActiveTexture(GL13.GL_TEXTURE0 + slot)
        GL11.glBindTexture(GL11.GL_TEXTURE_2D, buffer)
        callback()
        GL13.glActiveTexture(GL13.GL_TEXTURE0 + slot)
        GL11.glBindTexture(GL11.GL_TEXTURE_2D, 0)
        GL13.glActiveTexture(GL13.GL_TEXTURE0)
    }

    fun getSize(): Int2 {
        return Int2(size)
    }

    fun resize(size: Int2) {
        if (!this.size.equals(size)) {
            GL11.glBindTexture(GL11.GL_TEXTURE_2D, buffer)
            GL11.glTexImage2D(GL11.GL_TEXTURE_2D, 0, GL30.GL_RGBA32F, size.x, size.y, 0, GL11.GL_RGBA, GL11.GL_UNSIGNED_BYTE, null as ByteBuffer?)
            GL11.glBindTexture(GL11.GL_TEXTURE_2D, 0)
            this.size.set(size)
        }
    }

    companion object {
        fun generateTexture(size: Int2, pixels: ByteArray?): Int {
            val buffer = GL11.glGenTextures()
            GL11.glBindTexture(GL11.GL_TEXTURE_2D, buffer)
            GL11.glTexImage2D(GL11.GL_TEXTURE_2D, 0, GL30.GL_RGBA32F, size.x, size.y, 0, GL11.GL_RGBA, GL11.GL_UNSIGNED_BYTE, if (pixels != null) Memory.createByteBuffer(pixels) else null)
            GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_MIN_FILTER, GL11.GL_LINEAR)
            GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_MAG_FILTER, GL11.GL_LINEAR)
            GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_WRAP_S, GL11.GL_REPEAT)
            GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_WRAP_T, GL11.GL_REPEAT)
            GL11.glBindTexture(GL11.GL_TEXTURE_2D, 0)
            return buffer
        }

        fun use(textures: IntArray, callback: () -> Unit) {
            for (i in textures.indices) {
                GL13.glActiveTexture(GL13.GL_TEXTURE0 + i)
                GL11.glBindTexture(GL11.GL_TEXTURE_2D, textures[i])
            }
            callback()
            for (i in textures.indices) {
                GL13.glActiveTexture(GL13.GL_TEXTURE0 + i)
                GL11.glBindTexture(GL11.GL_TEXTURE_2D, 0)
            }
            GL13.glActiveTexture(GL13.GL_TEXTURE0)
        }
    }
}