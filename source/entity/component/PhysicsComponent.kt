package entity.component

import entity.Entity
import entity.abs.EntityComponent
import gui.helper.GUIEdit
import math.Float3
import java.io.Serializable

class PhysicsComponent(entity: Entity) : EntityComponent(entity), Serializable {
    val acceleration = Float3()
    val velocity = Float3()
    val angular = Float3()
    override fun renderGUI() {
        GUIEdit.editFloat3("Acceleration", acceleration, 0.1f)
        GUIEdit.editFloat3("Velocity", velocity, 0.1f)
        GUIEdit.editFloat3("Angular", angular, 0.1f)
    }
}