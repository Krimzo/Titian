package window.input

import imgui.ImGui

object Input {
    fun isMousePressed(key: Int): Boolean {
        return ImGui.isMouseClicked(key)
    }

    fun isMouseDown(key: Int): Boolean {
        return ImGui.isMouseDown(key)
    }

    fun isMouseReleased(key: Int): Boolean {
        return ImGui.isMouseReleased(key)
    }

    fun isKeyPressed(key: Int): Boolean {
        return ImGui.isKeyPressed(key)
    }

    fun isKeyDown(key: Int): Boolean {
        return ImGui.isKeyDown(key)
    }

    fun isKeyReleased(key: Int): Boolean {
        return ImGui.isKeyReleased(key)
    }

    val isShiftDown: Boolean
        get() = ImGui.getIO().keyShift
    val isCtrlDown: Boolean
        get() = ImGui.getIO().keyCtrl
    val isAltDown: Boolean
        get() = ImGui.getIO().keyAlt
}