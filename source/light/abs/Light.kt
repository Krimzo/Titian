package light.abs

import editor.Editor
import entity.Entity
import gui.helper.GUIEdit
import math.Float3
import named.NameHolder
import java.io.Serializable

abstract class Light(holder: NameHolder, name: String, editor: Editor) : Entity(holder, name, editor), Serializable {
    var color = Float3(1f)

    open fun fullLight(): Float3 {
        return color
    }

    override fun renderInfoGUI(editor: Editor) {
        super.renderInfoGUI(editor)

        GUIEdit.editColor3("Color", color)
    }
}