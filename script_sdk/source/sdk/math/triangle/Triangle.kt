package sdk.math.triangle

import sdk.math.cross
import sdk.math.dot
import sdk.math.normalize
import sdk.math.vector.Float2
import sdk.math.vector.Float3
import sdk.math.vector.Float4
import java.io.Serializable

class Triangle : Serializable {
    var a: Vertex
    var b: Vertex
    var c: Vertex

    // Construct
    constructor() : this(Vertex(), Vertex(), Vertex())

    constructor(a: Vertex, b: Vertex, c: Vertex) {
        this.a = a
        this.b = b
        this.c = c
    }

    // Properties
    val normal: Float3
        get() = normalize(cross(b.world - a.world, c.world - a.world))

    val constants: Float4
        get() {
            val calcConst = (b.world.y - c.world.y) * (a.world.x - c.world.x) + (c.world.x - b.world.x) * (a.world.y - c.world.y)
            val recConstant = 1f / calcConst

            return Float4(
                (b.world.y - c.world.y) * recConstant,
                (c.world.x - b.world.x) * recConstant,
                (c.world.y - a.world.y) * recConstant,
                (a.world.x - c.world.x) * recConstant,
            )
        }

    // Interpolation
    fun weights(constants: Float4, point: Float2): Float3 {
        val dx = point.x - c.world.x
        val dy = point.y - c.world.y
        val w1 = dx * constants.x + dy * constants.y
        val w2 = dx * constants.z + dy * constants.w
        return Float3(w1, w2, (1f - w1 - w2))
    }

    fun weights(position: Float3): Float3 {
        val v0 = a.world - c.world
        val v1 = b.world - c.world
        val v2 = position - c.world

        val d00 = dot(v0, v0)
        val d01 = dot(v0, v1)
        val d11 = dot(v1, v1)
        val d20 = dot(v2, v0)
        val d21 = dot(v2, v1)

        val inverseDenom = 1f / (d00 * d11 - d01 * d01)
        val w1 = (d11 * d20 - d01 * d21) * inverseDenom
        val w2 = (d00 * d21 - d01 * d20) * inverseDenom

        return Float3(w1, w2, (1f - w1 - w2))
    }

    fun interpolateSelf(weights: Float3): Vertex {
        return Vertex(
            Float3(
                interpolate(weights, Float3(a.world.x, b.world.x, c.world.x)),
                interpolate(weights, Float3(a.world.y, b.world.y, c.world.y)),
                interpolate(weights, Float3(a.world.z, b.world.z, c.world.z)),
            ),
            Float2(
                interpolate(weights, Float3(a.texture.x, b.texture.x, c.texture.x)),
                interpolate(weights, Float3(a.texture.y, b.texture.y, c.texture.y)),
            ),
            Float3(
                interpolate(weights, Float3(a.normal.x, b.normal.x, c.normal.x)),
                interpolate(weights, Float3(a.normal.y, b.normal.y, c.normal.y)),
                interpolate(weights, Float3(a.normal.z, b.normal.z, c.normal.z)),
            ),
        )
    }

    // Override
    override fun toString(): String {
        return "Triangle($a, $b, $c)"
    }

    // Static
    companion object {
        fun isInTriangle(weights: Float3): Boolean {
            return !(weights.x < 0f || weights.y < 0f || weights.z < 0f)
        }

        fun interpolate(weights: Float3, values: Float3): Float {
            return dot(weights, values)
        }
    }
}
