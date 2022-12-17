package math

import java.io.Serializable
import kotlin.math.acos
import kotlin.math.cos
import kotlin.math.sin
import kotlin.math.sqrt

class Float2 : Serializable {
    var x: Float = 0f
    var y: Float = 0f

    constructor(x: Float, y: Float) {
        this.x = x
        this.y = y
    }

    constructor() : this(0f, 0f)

    constructor(a: Float) : this(a, a)

    constructor(data: FloatArray) : this(data[0], data[1])

    constructor(v: Float2) : this(v.x, v.y)

    constructor(v: Int2) : this(v.x.toFloat(), v.y.toFloat())

    // Getters
    val length: Float
        get() = sqrt(this * this)

    val array: FloatArray
        get() = floatArrayOf(x, y)

    // Math
    operator fun plus(v: Float2): Float2 {
        return Float2(x + v.x, y + v.y)
    }

    operator fun minus(v: Float2): Float2 {
        return Float2(x - v.x, y - v.y)
    }

    operator fun times(a: Float): Float2 {
        return Float2(x * a, y * a)
    }

    operator fun times(v: Float2): Float {
        return x * v.x + y * v.y
    }

    operator fun div(a: Float): Float2 {
        return this * (1f / a)
    }

    operator fun unaryMinus(): Float2 {
        return this * -1f
    }

    override fun equals(other: Any?): Boolean {
        if (other is Float2) {
            return x == other.x && y == other.y
        }
        return false
    }

    fun angle(v: Float2): Float {
        return toDegrees(acos(normalize(this) * normalize(v)))
    }

    fun rotate(angle: Float): Float2 {
        val sinA = sin(toRadians(angle))
        val cosA = cos(toRadians(angle))
        return Float2(cosA * x - sinA * y, sinA * x + cosA * y)
    }

    override fun toString(): String {
        return "($x, $y)"
    }

    override fun hashCode(): Int {
        var result = x.hashCode()
        result = 31 * result + y.hashCode()
        return result
    }

    companion object {
        val posX: Float2
            get() = Float2(1f, 0f)
        val negX: Float2
            get() = Float2(-1f, 0f)
        val posY: Float2
            get() = Float2(0f, 1f)
        val negY: Float2
            get() = Float2(0f, -1f)
    }
}