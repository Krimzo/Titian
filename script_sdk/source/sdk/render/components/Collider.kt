package sdk.render.components

import sdk.math.vector.Float3
import sdk.render.components.shape.Shape

class Collider {
    var shape: Shape? = null

    // Geometry
    var rotation: Float3 = Float3()
    var offset: Float3 = Float3()

    // Material
    var staticFriction: Float = 0f
    var dynamicFriction: Float = 0f
    var restitution: Float = 0f
}
