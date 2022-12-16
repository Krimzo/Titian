package window

import math.Int2
import org.lwjgl.glfw.Callbacks
import org.lwjgl.glfw.GLFW
import org.lwjgl.glfw.GLFWImage
import org.lwjgl.system.MemoryUtil
import utility.Memory
import utility.helper.FileHelper

class Window(size: Int2, title: String, resizable: Boolean) {
    val context: GLContext
    var window: Long = 0
        private set

    init {
        if (!GLFW.glfwInit()) {
            throw Error("Unable to initialize GLFW")
        }
        GLFW.glfwDefaultWindowHints()
        GLFW.glfwWindowHint(GLFW.GLFW_VISIBLE, GLFW.GLFW_FALSE)
        GLFW.glfwWindowHint(GLFW.GLFW_RESIZABLE, if (resizable) GLFW.GLFW_TRUE else GLFW.GLFW_FALSE)
        GLFW.glfwWindowHint(GLFW.GLFW_DECORATED, GLFW.GLFW_FALSE)
        GLFW.glfwWindowHint(GLFW.GLFW_CONTEXT_VERSION_MAJOR, 3)
        GLFW.glfwWindowHint(GLFW.GLFW_CONTEXT_VERSION_MINOR, 3)
        GLFW.glfwWindowHint(GLFW.GLFW_OPENGL_PROFILE, GLFW.GLFW_OPENGL_CORE_PROFILE)
        GLFW.glfwWindowHint(GLFW.GLFW_OPENGL_FORWARD_COMPAT, GLFW.GLFW_TRUE)
        if (GLFW.glfwCreateWindow(size.x, size.y, title, MemoryUtil.NULL, MemoryUtil.NULL).also { window = it } == 0L) {
            throw Error("Failed to create a GLFW window")
        }
        GLFW.glfwMakeContextCurrent(window)
        context = GLContext()
        setHidden(false)
    }

    fun destroy() {
        context.destroy()
        GLFW.glfwMakeContextCurrent(MemoryUtil.NULL)
        Callbacks.glfwFreeCallbacks(window)
        GLFW.glfwDestroyWindow(window)
        GLFW.glfwTerminate()
        window = 0
    }

    fun process(): Boolean {
        if (!GLFW.glfwWindowShouldClose(window)) {
            GLFW.glfwPollEvents()
            return true
        }
        return false
    }

    fun close() {
        GLFW.glfwSetWindowShouldClose(window, true)
    }

    fun maximize() {
        GLFW.glfwMaximizeWindow(window)
    }

    fun minimize() {
        GLFW.glfwIconifyWindow(window)
    }

    fun restore() {
        GLFW.glfwRestoreWindow(window)
    }

    val isMaximized: Boolean
        get() = GLFW.glfwGetWindowAttrib(window, GLFW.GLFW_MAXIMIZED) != 0
    val size: Int2
        get() {
            val sizeX = IntArray(1)
            val sizeY = IntArray(1)
            GLFW.glfwGetWindowSize(window, sizeX, sizeY)
            return Int2(sizeX[0], sizeY[0])
        }
    var position: Int2
        get() {
            val posX = IntArray(1)
            val posY = IntArray(1)
            GLFW.glfwGetWindowPos(window, posX, posY)
            return Int2(posX[0], posY[0])
        }
        set(pos) {
            GLFW.glfwSetWindowPos(window, pos.x, pos.y)
        }

    fun setHidden(state: Boolean) {
        if (state) {
            GLFW.glfwHideWindow(window)
        } else {
            GLFW.glfwShowWindow(window)
        }
    }

    fun setMouseState(enabled: Boolean) {
        GLFW.glfwSetInputMode(window, GLFW.GLFW_CURSOR, if (enabled) GLFW.GLFW_CURSOR_NORMAL else GLFW.GLFW_CURSOR_DISABLED)
    }

    var mousePosition: Int2?
        get() {
            val position = arrayOf(DoubleArray(1), DoubleArray(1))
            GLFW.glfwGetCursorPos(window, position[0], position[1])
            return Int2(position[0][0].toInt(), position[1][0].toInt())
        }
        set(position) {
            GLFW.glfwSetCursorPos(window, position!!.x.toDouble(), position.y.toDouble())
        }

    fun swapBuffers() {
        GLFW.glfwSwapBuffers(window)
    }

    fun setTitle(text: String) {
        GLFW.glfwSetWindowTitle(window, text)
    }

    fun setVSync(enabled: Boolean) {
        GLFW.glfwSwapInterval(if (enabled) 1 else 0)
    }

    fun setIcon(filepath: String) {
        val imageSize = FileHelper.getImageSize(filepath)
        FileHelper.getImageData(filepath, false)?.let {
            val image = GLFWImage.create()
            image[imageSize.x, imageSize.y] = Memory.createByteBuffer(it)
            val imageBuffer = GLFWImage.create(1)
            imageBuffer.put(0, image)
            GLFW.glfwSetWindowIcon(window, imageBuffer)
        }
    }
}