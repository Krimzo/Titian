package sdk.math.vector

import sdk.math.dot
import sdk.math.imaginary.Complex
import java.io.Serializable
import kotlin.math.sqrt

class Float2 : Serializable {
    var x: Float
    var y: Float

    // Construct
    constructor() : this(0f)

    constructor(a: Float) : this(a, a)

    constructor(x: Float, y: Float) {
        this.x = x
        this.y = y
    }

    // Access
    operator fun get(index: Int): Float {
        return when (index) {
            0 -> x
            1 -> y
            else -> throw IndexOutOfBoundsException()
        }
    }

    operator fun set(index: Int, value: Float) {
        when (index) {
            0 -> x = value
            1 -> y = value
            else -> throw IndexOutOfBoundsException()
        }
    }

    // Cast
    val int2: Int2
        get() = Int2(x.toInt(), y.toInt())

    val complex: Complex
        get() = Complex()

    // Compare
    override fun equals(other: Any?): Boolean {
        if (other is Float2) {
            return (x == other.x && y == other.y)
        }
        return false
    }

    // Math
    operator fun plus(other: Float2): Float2 {
        return Float2(x + other.x, y + other.y)
    }

    operator fun minus(other: Float2): Float2 {
        return Float2(x - other.x, y - other.y)
    }

    operator fun times(value: Float): Float2 {
        return Float2(x * value, y * value)
    }

    operator fun times(other: Float2): Float2 {
        return Float2(x * other.x, y * other.y)
    }

    operator fun div(value: Float): Float2 {
        return Float2(x / value, y / value)
    }

    operator fun div(other: Float2): Float2 {
        return Float2(x / other.x, y / other.y)
    }

    // Other
    operator fun unaryMinus(): Float2 {
        return this * -1f
    }

    val length: Float
        get() = sqrt(dot(this, this))

    // Override
    override fun toString(): String {
        return "Float2($x, $y)"
    }

    override fun hashCode(): Int {
        var result = x.hashCode()
        result = 31 * result + y.hashCode()
        return result
    }

    // Constants
    companion object {
        val POSITIVE_X: Float2
            get() = Float2(1f, 0f)
        val POSITIVE_Y: Float2
            get() = Float2(0f, 1f)

        val NEGATIVE_X: Float2
            get() = Float2(-1f, 0f)
        val NEGATIVE_Y: Float2
            get() = Float2(0f, -1f)
    }
}
