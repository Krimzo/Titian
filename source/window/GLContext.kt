package window

import math.Float4
import math.Int2
import org.lwjgl.opengl.GL
import org.lwjgl.opengl.GL11

class GLContext {
    init {
        GL.createCapabilities()
    }

    fun destroy() {
        GL.setCapabilities(null)
    }

    fun setViewport(size: Int2?) {
        setViewport(Int2(), size)
    }

    fun setViewport(pos: Int2, size: Int2?) {
        GL11.glViewport(pos.x, pos.y, size!!.x, size.y)
    }

    fun setClearColor(color: Float4) {
        GL11.glClearColor(color.x, color.y, color.z, color.w)
    }

    fun clear(depth: Boolean) {
        GL11.glClear(GL11.GL_COLOR_BUFFER_BIT or if (depth) GL11.GL_DEPTH_BUFFER_BIT else 0)
    }

    fun setDepthTest(enabled: Boolean) {
        if (enabled) {
            GL11.glEnable(GL11.GL_DEPTH_TEST)
        } else {
            GL11.glDisable(GL11.GL_DEPTH_TEST)
        }
    }

    fun setWireframe(wireframe: Boolean) {
        GL11.glPolygonMode(GL11.GL_FRONT_AND_BACK, if (wireframe) GL11.GL_LINE else GL11.GL_FILL)
    }

    fun setCull(enabled: Boolean, cullBack: Boolean) {
        if (enabled) {
            GL11.glEnable(GL11.GL_CULL_FACE)
            GL11.glCullFace(if (cullBack) GL11.GL_BACK else GL11.GL_FRONT)
        } else {
            GL11.glDisable(GL11.GL_CULL_FACE)
        }
    }
}