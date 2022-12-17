package gui.section

import editor.Editor
import gui.abs.GUISection
import gui.helper.GUIEdit
import imgui.ImGui
import imgui.flag.ImGuiStyleVar

class GUIProfiling(editor: Editor) : GUISection(editor) {
    private var vSync: Boolean = true

    override fun renderGUI() {
        ImGui.pushStyleVar(ImGuiStyleVar.WindowPadding, 14f, 14f)

        if (ImGui.begin("Profiling")) {
            val deltaTime = editor.timer.getDeltaT()
            if (ImGui.checkbox("vSync", vSync)) {
                editor.window.setVSync(!vSync.also { vSync = it })
            }

            GUIEdit.editFloat("FPS", 1f / deltaTime, 0f)
            GUIEdit.editFloat("Frame", deltaTime, 0f)
        }
        ImGui.end()

        ImGui.popStyleVar()
    }
}