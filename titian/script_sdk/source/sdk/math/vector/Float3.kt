package sdk.math.vector

import sdk.math.dot
import sdk.math.helper.MathConstants
import sdk.math.imaginary.Quaternion
import java.awt.Color
import java.io.Serializable
import kotlin.math.sqrt

class Float3 : Serializable {
    var x: Float
    var y: Float
    var z: Float

    // Construct
    constructor() : this(0f)

    constructor(a: Float) : this(a, a, a)

    constructor(x: Float, y: Float, z: Float) {
        this.x = x
        this.y = y
        this.z = z
    }

    constructor(v: Float2, z: Float) : this(v.x, v.y, z)

    constructor(x: Float, v: Float2) : this(x, v.x, v.y)

    constructor(color: Color) {
        x = color.red * MathConstants.TO_FLOAT_COLOR
        y = color.green * MathConstants.TO_FLOAT_COLOR
        z = color.blue * MathConstants.TO_FLOAT_COLOR
    }

    // Access
    operator fun get(index: Int): Float {
        return when (index) {
            0 -> x
            1 -> y
            2 -> z
            else -> throw IndexOutOfBoundsException()
        }
    }

    operator fun set(index: Int, value: Float) {
        when (index) {
            0 -> x = value
            1 -> y = value
            2 -> z = value
            else -> throw IndexOutOfBoundsException()
        }
    }

    // Cast
    val xy: Float2
        get() = Float2(x, y)

    val color: Color
        get() = Color(
            (x * 255).toInt().coerceAtLeast(0).coerceAtMost(255),
            (y * 255).toInt().coerceAtLeast(0).coerceAtMost(255),
            (z * 255).toInt().coerceAtLeast(0).coerceAtMost(255),
        )

    val quaternion: Quaternion
        get() = Quaternion(x, y, z)

    // Compare
    override fun equals(other: Any?): Boolean {
        if (other is Float3) {
            return (x == other.x && y == other.y && z == other.z)
        }
        return false
    }

    // Math
    operator fun plus(other: Float3): Float3 {
        return Float3(x + other.x, y + other.y, z + other.z)
    }

    operator fun minus(other: Float3): Float3 {
        return Float3(x - other.x, y - other.y, z - other.z)
    }

    operator fun times(value: Float): Float3 {
        return Float3(x * value, y * value, z * value)
    }

    operator fun times(other: Float3): Float3 {
        return Float3(x * other.x, y * other.y, z * other.z)
    }

    operator fun div(value: Float): Float3 {
        return Float3(x / value, y / value, z / value)
    }

    operator fun div(other: Float3): Float3 {
        return Float3(x / other.x, y / other.y, z / other.z)
    }

    // Other
    operator fun unaryMinus(): Float3 {
        return this * -1f
    }

    val length: Float
        get() = sqrt(dot(this, this))

    // Override
    override fun toString(): String {
        return "Float3($x, $y, $z)"
    }

    override fun hashCode(): Int {
        var result = x.hashCode()
        result = 31 * result + y.hashCode()
        result = 31 * result + z.hashCode()
        return result
    }

    // Constants
    companion object {
        val POSITIVE_X: Float3
            get() = Float3(1f, 0f, 0f)
        val POSITIVE_Y: Float3
            get() = Float3(0f, 1f, 0f)
        val POSITIVE_Z: Float3
            get() = Float3(0f, 0f, 1f)

        val NEGATIVE_X: Float3
            get() = Float3(-1f, 0f, 0f)
        val NEGATIVE_Y: Float3
            get() = Float3(0f, -1f, 0f)
        val NEGATIVE_Z: Float3
            get() = Float3(0f, 0f, -1f)
    }
}
