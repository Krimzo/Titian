package light

import editor.Editor
import gui.helper.GUIEdit
import imgui.ImGui
import light.abs.Light
import math.Float3
import named.NameHolder
import java.io.Serializable

class AmbientLight(holder: NameHolder, name: String, editor: Editor) : Light(holder, name, editor), Serializable {
    var intensity: Float = 0.1f

    override fun fullLight(): Float3 {
        return super.fullLight() * intensity
    }

    override fun renderInfoGUI(editor: Editor) {
        super.renderInfoGUI(editor)

        intensity = GUIEdit.editFloat("Intensity", intensity, 0.1f)

        val isMain = editor.scene.selected.ambientLight === this
        if (ImGui.checkbox("Main ambient light", isMain)) {
            editor.scene.selected.ambientLight = if (isMain) null else this
        }
    }
}