package light

import editor.Editor
import gui.helper.GUIEdit
import imgui.ImGui
import light.abs.Light
import math.Float3
import named.NameHolder
import java.io.Serializable

class DirectionalLight(holder: NameHolder?, name: String?, editor: Editor?) : Light(holder, name, editor), Serializable {
    var direction: Float3 = Float3.negZ
        private set

    fun setDirection(direction: Float3) {
        this.direction.set(direction.normalize())
    }

    override fun renderInfoGUI(editor: Editor) {
        super.renderInfoGUI(editor)
        GUIEdit.editFloat3("Direction", direction, 0.1f)
        direction.set(direction.normalize())
        val isMain = editor.scene?.selected?.directionalLight === this
        if (ImGui.checkbox("Main directional light", isMain)) {
            editor.scene?.selected?.directionalLight = if (isMain) null else this
        }
    }
}