package gui.helper

import imgui.ImGui
import imgui.flag.ImGuiInputTextFlags
import imgui.flag.ImGuiWindowFlags
import imgui.type.ImString

class GUITextInput(text: String?, private val onEdit: (String) -> Unit) {
    private val id: Int = idCounter++
    private val text: ImString

    init {
        this.text = ImString(text, 100)
    }

    fun update() {
        if (ImGui.begin("Text input##$id", ImGuiWindowFlags.NoCollapse or ImGuiWindowFlags.NoScrollbar)) {
            ImGui.setNextItemWidth(-1f)
            if (ImGui.inputText("##Input", text, ImGuiInputTextFlags.EnterReturnsTrue)) {
                val text = text.toString()
                if (text.isNotEmpty()) {
                    onEdit(text)
                }
            }
        }
        ImGui.end()
    }

    companion object {
        private var idCounter = 0
    }
}