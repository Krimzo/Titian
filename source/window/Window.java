package window;

import callback.ResizeCallback;
import utility.Disposable;
import math.Int2;
import org.lwjgl.glfw.GLFWImage;
import utility.File;
import utility.Memory;
import window.input.*;

import static org.lwjgl.glfw.Callbacks.*;
import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.system.MemoryUtil.NULL;

public class Window implements Disposable {
    private long window;
    private GLContext context;

    public final Keyboard keyboard;
    public final Mouse mouse;

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

        keyboard = new Keyboard(window);
        mouse = new Mouse(window);
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
        boolean running = !glfwWindowShouldClose(window);
        if (running) {
            glfwPollEvents();
            keyboard.update();
            mouse.update();
        }
        return running;
    }

    public void close() {
        glfwSetWindowShouldClose(window, true);
    }

    public void setOnResize(ResizeCallback callback) {
        glfwSetFramebufferSizeCallback(window, (long window, int width, int height) -> {
            callback.method(new Int2(width, height));
        });
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
        Int2 imageSize = File.getImageSize(filepath);
        byte[] imageData = File.getImageData(filepath, false);

        if (imageData != null) {
            GLFWImage image = GLFWImage.create();
            image.set(imageSize.x, imageSize.y, Memory.createByteBuffer(imageData));

            GLFWImage.Buffer imageBuffer = GLFWImage.create(1);
            imageBuffer.put(0, image);
            glfwSetWindowIcon(window, imageBuffer);
        }
    }
}
