package window.input;

import math.Int2;

import static org.lwjgl.glfw.GLFW.*;

public class Mouse {
    private final long window;
    protected Int2 position = new Int2();
    protected int scroll = 0;
    public Key lmb = new Key();
    public Key mmb = new Key();
    public Key rmb = new Key();

    public Mouse(long window) {
        this.window = window;

        glfwSetScrollCallback(window, (long win, double deltaStart, double deltaEnd) -> {
            scroll = (int)(deltaEnd - deltaStart);
        });
    }

    public void setHidden(boolean state) {
        glfwSetInputMode(window, GLFW_CURSOR, state ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    }

    public Int2 getPosition() {
        return position;
    }

    public void setPosition(Int2 pos) {
        glfwSetCursorPos(window, pos.x, pos.y);
    }

    public int getScroll() {
        return scroll;
    }

    public void update() {
        double[] posX = new double[1]; double[] posY = new double[1];
        glfwGetCursorPos(window, posX, posY);
        position = new Int2((int)posX[0], (int)posY[0]);
        lmb.update(Key.getMouseState(window, GLFW_MOUSE_BUTTON_LEFT));
        mmb.update(Key.getMouseState(window, GLFW_MOUSE_BUTTON_MIDDLE));
        rmb.update(Key.getMouseState(window, GLFW_MOUSE_BUTTON_RIGHT));
    }
}
