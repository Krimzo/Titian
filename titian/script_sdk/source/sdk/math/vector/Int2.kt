package sdk.math.vector

import java.io.Serializable

class Int2 : Serializable {
    var x: Int
    var y: Int

    // Construct
    constructor() : this(0)

    constructor(a: Int) : this(a, a)

    constructor(x: Int, y: Int) {
        this.x = x
        this.y = y
    }

    // Access
    operator fun get(index: Int): Int {
        return when (index) {
            0 -> x
            1 -> y
            else -> throw IndexOutOfBoundsException()
        }
    }

    operator fun set(index: Int, value: Int) {
        when (index) {
            0 -> x = value
            1 -> y = value
            else -> throw IndexOutOfBoundsException()
        }
    }

    // Cast
    val float2: Float2
        get() = Float2(x.toFloat(), y.toFloat())

    // Compare
    override fun equals(other: Any?): Boolean {
        if (other is Int2) {
            return (x == other.x && y == other.y)
        }
        return false
    }

    // Math
    operator fun plus(other: Int2): Int2 {
        return Int2(x + other.x, y + other.y)
    }

    operator fun minus(other: Int2): Int2 {
        return Int2(x - other.x, y - other.y)
    }

    operator fun times(value: Int): Int2 {
        return Int2(x * value, y * value)
    }

    operator fun times(other: Int2): Int2 {
        return Int2(x * other.x, y * other.y)
    }

    operator fun div(value: Int): Int2 {
        return Int2(x / value, y / value)
    }

    operator fun div(other: Int2): Int2 {
        return Int2(x / other.x, y / other.y)
    }

    // Other
    operator fun unaryMinus(): Int2 {
        return this * -1
    }

    // Override
    override fun toString(): String {
        return "Int2($x, $y)"
    }

    override fun hashCode(): Int {
        var result = x
        result = 31 * result + y
        return result
    }

    // Constants
    companion object {
        val POSITIVE_X: Int2
            get() = Int2(1, 0)
        val POSITIVE_Y: Int2
            get() = Int2(0, 1)

        val NEGATIVE_X: Int2
            get() = Int2(-1, 0)
        val NEGATIVE_Y: Int2
            get() = Int2(0, -1)
    }
}
