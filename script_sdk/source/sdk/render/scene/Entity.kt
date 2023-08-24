package sdk.render.scene

import sdk.math.vector.Float3
import sdk.render.components.Collider

class Entity {
    // Render
    var renderScale: Float3 = Float3()
    var mesh: String? = null
    var material: String? = null

    // Geometry
    var rotation: Float3 = Float3()
    var position: Float3 = Float3()

    // Physics
    var dynamic: Boolean = false
    var gravity: Boolean = false
    var mass: Float = 0f
    var velocity: Float3 = Float3()
    var angular: Float3 = Float3()

    // Collision
    var collider: Collider? = null
}
