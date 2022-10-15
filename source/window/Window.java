package window;

import math.Int2;
import org.lwjgl.glfw.GLFWImage;
import utility.Files;
import utility.Instance;
import utility.Memory;

import static org.lwjgl.glfw.Callbacks.glfwFreeCallbacks;
import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.system.MemoryUtil.NULL;

public class Window {
    private final GLContext context;
    private long window;

    public Window(Int2 size, String title, boolean resizable) {
        if (!glfwInit()) {
            throw new Error("Unable to initialize GLFW");
        }

        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

        if ((window = glfwCreateWindow(size.x, size.y, title, NULL, NULL)) == 0) {
            throw new Error("Failed to create a GLFW window");
        }

        glfwMakeContextCurrent(window);
        context = new GLContext();

        setHidden(false);
    }

    public void destroy() {
        context.destroy();
        glfwMakeContextCurrent(NULL);
        glfwFreeCallbacks(window);
        glfwDestroyWindow(window);
        glfwTerminate();
        window = 0;
    }

    public long getWindow() {
        return window;
    }

    public GLContext getContext() {
        return context;
    }

    public boolean process() {
        if (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
            return true;
        }
        return false;
    }

    public void close() {
        glfwSetWindowShouldClose(window, true);
    }

    public void maximize() {
        glfwMaximizeWindow(window);
    }

    public void minimize() {
        glfwIconifyWindow(window);
    }

    public void restore() {
        glfwRestoreWindow(window);
    }

    public boolean isMaximized() {
        return glfwGetWindowAttrib(window, GLFW_MAXIMIZED) != 0;
    }

    public Int2 getSize() {
        int[] sizeX = new int[1];
        int[] sizeY = new int[1];
        glfwGetWindowSize(window, sizeX, sizeY);
        return new Int2(sizeX[0], sizeY[0]);
    }

    public Int2 getPosition() {
        int[] posX = new int[1];
        int[] posY = new int[1];
        glfwGetWindowPos(window, posX, posY);
        return new Int2(posX[0], posY[0]);
    }

    public void setPosition(Int2 pos) {
        glfwSetWindowPos(window, pos.x, pos.y);
    }

    public void setHidden(boolean state) {
        if (state) {
            glfwHideWindow(window);
        }
        else {
            glfwShowWindow(window);
        }
    }

    public void setMouseState(boolean enabled) {
        glfwSetInputMode(window, GLFW_CURSOR, enabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    }

    public Int2 getMousePosition() {
        double[][] position = { new double[1], new double[1] };
        glfwGetCursorPos(window, position[0], position[1]);
        return new Int2((int) position[0][0], (int) position[1][0]);
    }

    public void setMousePosition(Int2 position) {
        glfwSetCursorPos(window, position.x, position.y);
    }

    public void swapBuffers() {
        glfwSwapBuffers(window);
    }

    public void setTitle(String text) {
        glfwSetWindowTitle(window, text);
    }

    public void setVSync(boolean enabled) {
        glfwSwapInterval(enabled ? 1 : 0);
    }

    public void setIcon(String filepath) {
        Int2 imageSize = Files.getImageSize(filepath);
        byte[] imageData = Files.getImageData(filepath, false);

        if (Instance.isValid(imageData)) {
            GLFWImage image = GLFWImage.create();
            image.set(imageSize.x, imageSize.y, Memory.createByteBuffer(imageData));

            GLFWImage.Buffer imageBuffer = GLFWImage.create(1);
            imageBuffer.put(0, image);
            glfwSetWindowIcon(window, imageBuffer);
        }
    }
}
