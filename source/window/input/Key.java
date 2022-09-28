package window.input;

import callbacks.EmptyCallback;

import static org.lwjgl.glfw.GLFW.*;

public class Key {
    private boolean state = false;
    public EmptyCallback onPress = () -> {};
    public EmptyCallback onDown = () -> {};
    public EmptyCallback onRelease = () -> {};

    protected Key() {}

    public boolean getState() {
        return state;
    }

    protected static boolean getKeyState(long window, int key) {
        return glfwGetKey(window, key) == GLFW_PRESS;
    }

    protected static boolean getMouseState(long window, int button) {
        return glfwGetMouseButton(window, button) == GLFW_PRESS;
    }

    protected void update(boolean newState) {
        if (state) {
            if (!newState) {
                onRelease.method();
            }
            else {
                onDown.method();
            }
        }
        else if (newState) {
            onPress.method();
        }
        state = newState;
    }
}
