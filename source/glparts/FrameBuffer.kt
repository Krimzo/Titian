package glparts

import glparts.abs.GLObject
import math.Float4
import math.Int2
import org.lwjgl.opengl.GL11
import org.lwjgl.opengl.GL30
import window.GLContext

class FrameBuffer(context: GLContext?, size: Int2) : GLObject(null, null, context) {
    private val colorMap: Texture
    private val depthMap: DepthTexture
    private var buffer: Int

    init {
        colorMap = Texture(null, null, context, size, null)
        depthMap = DepthTexture(context, size)
        buffer = GL30.glGenFramebuffers()
        GL30.glBindFramebuffer(GL30.GL_FRAMEBUFFER, buffer)
        GL30.glFramebufferTexture2D(GL30.GL_FRAMEBUFFER, GL30.GL_COLOR_ATTACHMENT0, GL11.GL_TEXTURE_2D, colorMap.buffer, 0)
        GL30.glFramebufferTexture2D(GL30.GL_FRAMEBUFFER, GL30.GL_DEPTH_ATTACHMENT, GL11.GL_TEXTURE_2D, depthMap.buffer, 0)
        GL30.glBindFramebuffer(GL30.GL_FRAMEBUFFER, 0)
    }

    override fun deallocate() {
        GL30.glDeleteFramebuffers(buffer)
        buffer = 0
    }

    override fun isAllocated(): Boolean {
        return buffer != 0
    }

    fun clear() {
        GL30.glBindFramebuffer(GL30.GL_FRAMEBUFFER, buffer)
        GL11.glClear(GL11.GL_COLOR_BUFFER_BIT or GL11.GL_DEPTH_BUFFER_BIT)
        GL30.glBindFramebuffer(GL30.GL_FRAMEBUFFER, 0)
    }

    fun use(callback: () -> Unit) {
        GL30.glBindFramebuffer(GL30.GL_FRAMEBUFFER, buffer)
        callback()
        GL30.glBindFramebuffer(GL30.GL_FRAMEBUFFER, 0)
    }

    fun getColorMap(): Texture {
        return colorMap
    }

    fun getDepthMap(): DepthTexture {
        return depthMap
    }

    fun resize(size: Int2) {
        if (size.x > 0 && size.y > 0) {
            colorMap.resize(size)
            depthMap.resize(size)
        }
    }

    fun getPixel(pos: Int2): Float4? {
        return getPixels(pos, Int2(1))[0]
    }

    fun getPixels(pos: Int2, size: Int2): Array<Float4?> {
        val result = arrayOfNulls<Float4>(size.x * size.y)
        val data = FloatArray(result.size * 4)
        GL30.glBindFramebuffer(GL30.GL_FRAMEBUFFER, buffer)
        GL11.glReadPixels(pos.x, colorMap.getSize().y - 1 - pos.y, size.x, size.y, GL11.GL_RGBA, GL11.GL_FLOAT, data)
        GL30.glBindFramebuffer(GL30.GL_FRAMEBUFFER, 0)
        for (i in result.indices) {
            result[i] = Float4(
                    data[i * 4],
                    data[i * 4 + 1],
                    data[i * 4 + 2],
                    data[i * 4 + 3]
            )
        }
        return result
    }
}