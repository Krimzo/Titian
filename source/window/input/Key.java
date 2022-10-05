package window.input;

import callback.EmptyCallback;

import java.util.ArrayList;

import static org.lwjgl.glfw.GLFW.*;

public class Key {
    private boolean state = false;
    public final ArrayList<EmptyCallback> onPress = new ArrayList<>();
    public final ArrayList<EmptyCallback> onHold = new ArrayList<>();
    public final ArrayList<EmptyCallback> onRelease = new ArrayList<>();

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
                for (EmptyCallback callback : onRelease) {
                    callback.method();
                }
            }
            else {
                for (EmptyCallback callback : onHold) {
                    callback.method();
                }
            }
        }
        else if (newState) {
            for (EmptyCallback callback : onPress) {
                callback.method();
            }
        }
        state = newState;
    }
}
