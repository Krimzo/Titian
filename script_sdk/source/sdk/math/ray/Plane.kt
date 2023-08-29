package sdk.math.ray

import sdk.math.dot
import sdk.math.normalize
import sdk.math.vector.Float3
import java.io.Serializable

class Plane : Serializable {
    var origin: Float3
    var normal: Float3 = Float3()
        set(value) {
            field = normalize(value)
        }

    // Construct
    constructor() : this (Float3(), Float3.POSITIVE_Y)

    constructor(origin: Float3, normal: Float3) {
        this.origin = origin
        this.normal = normal
    }

    // Math
    fun inFront(point: Float3): Boolean {
        val result = dot(point - origin, normal)
        return (result >= 0f)
    }

    // Override
    override fun toString(): String {
        return "Plane($origin, $normal)"
    }
}
