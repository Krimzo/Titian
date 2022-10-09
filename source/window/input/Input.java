package window.input;

import imgui.ImGui;

public class Input {
    private Input() {}

    public static boolean isMousePressed(int key) {
        return ImGui.isMouseClicked(key);
    }
    public static boolean isMouseDown(int key) {
        return ImGui.isMouseDown(key);
    }
    public static boolean isMouseReleased(int key) {
        return ImGui.isMouseReleased(key);
    }

    public static boolean isKeyPressed(int key) {
        return ImGui.isKeyPressed(key);
    }
    public static boolean isKeyDown(int key) {
        return ImGui.isKeyDown(key);
    }
    public static boolean isKeyReleased(int key) {
        return ImGui.isKeyReleased(key);
    }

    public static boolean isShiftDown() {
        return ImGui.getIO().getKeyShift();
    }
    public static boolean isCtrlDown() {
        return ImGui.getIO().getKeyCtrl();
    }
    public static boolean isAlDown() {
        return ImGui.getIO().getKeyAlt();
    }
}
