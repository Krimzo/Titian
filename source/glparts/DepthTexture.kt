package glparts

import glparts.abs.GLObject
import math.Int2
import named.NameHolder
import org.lwjgl.opengl.GL11
import org.lwjgl.opengl.GL12
import org.lwjgl.opengl.GL13
import org.lwjgl.opengl.GL14
import window.GLContext
import java.nio.ByteBuffer

class DepthTexture(context: GLContext, size: Int2) : GLObject(NameHolder(), "Depth Texture", context) {
    private var size: Int2 = Int2()

    @Transient
    var buffer: Int = 0
        private set

    init {
        this.size = Int2(size)
        buffer = GL11.glGenTextures()

        GL11.glBindTexture(GL11.GL_TEXTURE_2D, buffer)
        GL11.glTexImage2D(GL11.GL_TEXTURE_2D, 0, GL11.GL_DEPTH_COMPONENT, size.x, size.y, 0, GL11.GL_DEPTH_COMPONENT, GL11.GL_FLOAT, null as ByteBuffer?)

        GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_MAG_FILTER, GL11.GL_NEAREST)
        GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_MIN_FILTER, GL11.GL_NEAREST)

        GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_WRAP_S, GL12.GL_CLAMP_TO_EDGE)
        GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL11.GL_TEXTURE_WRAP_T, GL12.GL_CLAMP_TO_EDGE)

        GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL14.GL_TEXTURE_COMPARE_FUNC, GL11.GL_LEQUAL)
        GL11.glTexParameteri(GL11.GL_TEXTURE_2D, GL14.GL_TEXTURE_COMPARE_MODE, GL11.GL_NONE)
        GL11.glBindTexture(GL11.GL_TEXTURE_2D, 0)
    }

    override fun deallocate() {
        GL11.glDeleteTextures(buffer)
        buffer = 0
    }

    override fun isAllocated(): Boolean {
        return buffer != 0
    }

    fun use(callback: () -> Unit) {
        GL11.glBindTexture(GL11.GL_TEXTURE_2D, buffer)
        callback()
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
        if (this.size != size) {
            GL11.glBindTexture(GL11.GL_TEXTURE_2D, buffer)
            GL11.glTexImage2D(GL11.GL_TEXTURE_2D, 0, GL11.GL_DEPTH_COMPONENT, size.x, size.y, 0, GL11.GL_DEPTH_COMPONENT, GL11.GL_FLOAT, null as ByteBuffer?)
            GL11.glBindTexture(GL11.GL_TEXTURE_2D, 0)
            this.size = Int2(size)
        }
    }
}