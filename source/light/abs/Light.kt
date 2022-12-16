package light.abs

import editor.Editor
import entity.Entity
import gui.helper.GUIEdit
import math.Float3
import named.NameHolder
import java.io.Serializable

abstract class Light(holder: NameHolder?, name: String?, editor: Editor?) : Entity(holder, name, editor), Serializable {
    private val color = Float3(1f)
    fun setColor(color: Float3?) {
        this.color.set(color)
    }

    open fun getColor(): Float3? {
        return Float3(color)
    }

    override fun renderInfoGUI(editor: Editor) {
        super.renderInfoGUI(editor)
        GUIEdit.editColor3("Color", color)
    }
}