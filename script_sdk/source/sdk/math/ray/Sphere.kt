package sdk.math.ray

import sdk.math.vector.Float3
import java.io.Serializable

class Sphere : Serializable {
    var origin: Float3
    var radius: Float

    // Construct
    constructor() : this(Float3(), 0f)

    constructor(origin: Float3, radius: Float) {
        this.origin = origin
        this.radius = radius
    }

    // Override
    override fun toString(): String {
        return "Sphere($origin, $radius)"
    }
}