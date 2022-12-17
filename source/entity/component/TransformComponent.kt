package entity.component

import entity.Entity
import entity.abs.EntityComponent
import gui.helper.GUIEdit
import math.Float3
import math.Mat4
import java.io.Serializable

class TransformComponent(entity: Entity) : EntityComponent(entity), Serializable {
    var scale: Float3 = Float3(1f)
    var rotation: Float3 = Float3()
    var position: Float3 = Float3()

    fun scalingMatrix(): Mat4 {
        return Mat4.scaling(scale)
    }

    fun rotationMatrix(): Mat4 {
        return Mat4.rotation(rotation)
    }

    fun translationMatrix(): Mat4 {
        return Mat4.translation(position)
    }

    fun matrix(): Mat4 {
        return translationMatrix() * rotationMatrix() * scalingMatrix()
    }

    override fun renderGUI() {
        GUIEdit.editFloat3("Scale", scale, 0.1f)
        GUIEdit.editFloat3("Rotation", rotation, 0.1f)
        GUIEdit.editFloat3("Position", position, 0.1f)
    }
}