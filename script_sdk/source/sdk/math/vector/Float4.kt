package sdk.math.vector

import sdk.math.dot
import sdk.math.imaginary.Quaternion
import java.awt.Color
import java.io.Serializable
import kotlin.math.sqrt

class Float4 : Serializable {
    var x: Float
    var y: Float
    var z: Float
    var w: Float

    // Construct
    constructor() : this(0f)

    constructor(a: Float) : this(a, a, a, a)

    constructor(x: Float, y: Float, z: Float, w: Float) {
        this.x = x
        this.y = y
        this.z = z
        this.w = w
    }

    constructor(v: Float2, z: Float, w: Float) : this(v.x, v.y, z, w)

    constructor(x: Float, v: Float2, w: Float) : this(x, v.x, v.y, w)

    constructor(x: Float, y: Float, v: Float2) : this(x, y, v.x, v.y)

    constructor(v1: Float2, v2: Float2) : this(v1.x, v1.y, v2.x, v2.y)

    constructor(v: Float3, w: Float) : this(v.x, v.y, v.z, w)

    constructor(x: Float, v: Float3) : this(x, v.x, v.y, v.z)

    constructor(color: Color) : this(Float3(color), color.alpha / 255f)

    // Access
    operator fun get(index: Int): Float {
        return when (index) {
            0 -> x
            1 -> y
            2 -> z
            3 -> w
            else -> throw IndexOutOfBoundsException()
        }
    }

    operator fun set(index: Int, value: Float) {
        when (index) {
            0 -> x = value
            1 -> y = value
            2 -> z = value
            3 -> w = value
            else -> throw IndexOutOfBoundsException()
        }
    }

    // Cast
    val xy: Float2
        get() = Float2(x, y)

    val xyz: Float3
        get() = Float3(x, y, z)

    val color: Color
        get() = Color(
            (x * 255).toInt().coerceAtLeast(0).coerceAtMost(255),
            (y * 255).toInt().coerceAtLeast(0).coerceAtMost(255),
            (z * 255).toInt().coerceAtLeast(0).coerceAtMost(255),
            (w * 255).toInt().coerceAtLeast(0).coerceAtMost(255),
        )

    val quaternion: Quaternion
        get() = Quaternion(w, x, y, z)

    // Compare
    override fun equals(other: Any?): Boolean {
        if (other is Float4) {
            return (x == other.x && y == other.y && z == other.z && w == other.w)
        }
        return false
    }

    // Math
    operator fun plus(other: Float4): Float4 {
        return Float4(x + other.x, y + other.y, z + other.z, w + other.w)
    }

    operator fun minus(other: Float4): Float4 {
        return Float4(x - other.x, y - other.y, z - other.z, w - other.w)
    }

    operator fun times(value: Float): Float4 {
        return Float4(x * value, y * value, z * value, w * value)
    }

    operator fun times(other: Float4): Float4 {
        return Float4(x * other.x, y * other.y, z * other.z, w * other.w)
    }

    operator fun div(value: Float): Float4 {
        return Float4(x / value, y / value, z / value, w / value)
    }

    operator fun div(other: Float4): Float4 {
        return Float4(x / other.x, y / other.y, z / other.z, w / other.w)
    }

    // Other
    operator fun unaryMinus(): Float4 {
        return this * -1f
    }

    val length: Float
        get() = sqrt(dot(this, this))

    // Override
    override fun toString(): String {
        return "Float4($x, $y, $z, $w)"
    }

    override fun hashCode(): Int {
        var result = x.hashCode()
        result = 31 * result + y.hashCode()
        result = 31 * result + z.hashCode()
        result = 31 * result + w.hashCode()
        return result
    }

    // Constants
    companion object {
        val POSITIVE_X: Float4
            get() = Float4(1f, 0f, 0f, 0f)
        val POSITIVE_Y: Float4
            get() = Float4(0f, 1f, 0f, 0f)
        val POSITIVE_Z: Float4
            get() = Float4(0f, 0f, 1f, 0f)
        val POSITIVE_W: Float4
            get() = Float4(0f, 0f, 0f, 1f)

        val NEGATIVE_X: Float4
            get() = Float4(-1f, 0f, 0f, 0f)
        val NEGATIVE_Y: Float4
            get() = Float4(0f, -1f, 0f, 0f)
        val NEGATIVE_Z: Float4
            get() = Float4(0f, 0f, -1f, 0f)
        val NEGATIVE_W: Float4
            get() = Float4(0f, 0f, 0f, -1f)
    }
}
