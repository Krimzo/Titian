package sdk.math.triangle

import sdk.math.vector.Float2
import sdk.math.vector.Float3
import java.io.Serializable

class Vertex : Serializable {
    var world: Float3
    var texture: Float2
    var normal: Float3

    // Construct
    constructor() : this(Float3(), Float2(), Float3())

    constructor(world: Float3) : this(world, Float2(), Float3())

    constructor(world: Float3, texture: Float2) : this(world, texture, Float3())

    constructor(world: Float3, normal: Float3) : this(world, Float2(), normal)

    constructor(world: Float3, texture: Float2, normal: Float3) {
        this.world = world
        this.texture = texture
        this.normal = normal
    }

    // Override
    override fun toString(): String {
        return "Vertex($world, $texture, $normal)"
    }
}