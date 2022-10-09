package window;

import glparts.abs.Disposable;
import math.Int2;
import org.lwjgl.glfw.GLFWImage;
import utility.Files;
import utility.Memory;
import glparts.abs.GLContext;

import static org.lwjgl.glfw.Callbacks.*;
import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.system.MemoryUtil.NULL;

public class Window implements Disposable {
    private long window;
    private GLContext context;

    public Window(Int2 size, String title, boolean resizable) {
        if (!glfwInit()) {
            throw new Error("Unable to initialize GLFW");
        }

        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);

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

    @Override
    public void dispose() {
        if (window != 0) {
            glfwMakeContextCurrent(NULL);
            context.dispose();
            context = null;

            glfwFreeCallbacks(window);
            glfwDestroyWindow(window);

            glfwTerminate();
            window = 0;
        }
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

        if (imageData != null) {
            GLFWImage image = GLFWImage.create();
            image.set(imageSize.x, imageSize.y, Memory.createByteBuffer(imageData));

            GLFWImage.Buffer imageBuffer = GLFWImage.create(1);
            imageBuffer.put(0, image);
            glfwSetWindowIcon(window, imageBuffer);
        }
    }
}
