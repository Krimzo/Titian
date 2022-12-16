package entity

import entity.component.*
import java.io.Serializable

class EntityComponents(entity: Entity) : Serializable {
    val transform: TransformComponent
    val mesh: MeshComponent
    val material: MaterialComponent
    val physics: PhysicsComponent
    val script: ScriptComponent

    init {
        transform = TransformComponent(entity)
        mesh = MeshComponent(entity)
        material = MaterialComponent(entity)
        physics = PhysicsComponent(entity)
        script = ScriptComponent(entity)
    }
}