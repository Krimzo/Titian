package math

import java.awt.Color
import java.io.Serializable
import kotlin.math.acos
import kotlin.math.sqrt

class Float4 : Serializable {
    var x: Float = 0f
    var y: Float = 0f
    var z: Float = 0f
    var w: Float = 0f

    constructor(x: Float, y: Float, z: Float, w: Float) {
        this.x = x
        this.y = y
        this.z = z
        this.w = w
    }

    constructor() : this(0f, 0f, 0f, 0f)

    constructor(a: Float) : this(a, a, a, a)

    constructor(data: FloatArray) : this(data[0], data[1], data[2], data[3])

    constructor(v: Float2, z: Float, w: Float) : this(v.x, v.y, z, w)

    constructor(x: Float, v: Float2, w: Float) : this(x, v.x, v.y, w)

    constructor(x: Float, y: Float, v: Float2) : this(x, y, v.x, v.y)

    constructor(v1: Float2, v2: Float2) : this(v1.x, v1.y, v2.x, v2.y)

    constructor(v: Float3, w: Float) : this(v.x, v.y, v.z, w)

    constructor(x: Float, v: Float3) : this(x, v.x, v.y, v.z)

    constructor(v: Float4) : this(v.x, v.y, v.z, v.w)

    constructor(v: Int4) : this(v.x.toFloat(), v.y.toFloat(), v.z.toFloat(), v.w.toFloat())

    constructor(color: Color) : this(Float3(color), color.alpha / 255f)

    // Getters
    val length: Float
        get() = sqrt(this * this)

    val xy: Float2
        get() = Float2(x, y)

    val xyz: Float3
        get() = Float3(x, y, z)

    val array: FloatArray
        get() = floatArrayOf(x, y, z, w)

    val color: Color
        get() = Color(
            (x * 255).toInt().coerceAtLeast(0).coerceAtMost(255),
            (y * 255).toInt().coerceAtLeast(0).coerceAtMost(255),
            (z * 255).toInt().coerceAtLeast(0).coerceAtMost(255),
            (w * 255).toInt().coerceAtLeast(0).coerceAtMost(255)
        )

    // Math
    operator fun plus(v: Float4): Float4 {
        return Float4(x + v.x, y + v.y, z + v.z, w + v.w)
    }

    operator fun minus(v: Float4): Float4 {
        return Float4(x - v.x, y - v.y, z - v.z, w - v.w)
    }

    operator fun times(a: Float): Float4 {
        return Float4(x * a, y * a, z * a, w * a)
    }

    operator fun times(v: Float4): Float {
        return x * v.x + y * v.y + z * v.z + w * v.w
    }

    operator fun div(a: Float): Float4 {
        return this * (1f / a)
    }

    operator fun unaryMinus(): Float4 {
        return this * -1f
    }

    override fun equals(other: Any?): Boolean {
        if (other is Float4) {
            return x == other.x && y == other.y && z == other.z && w == other.w
        }
        return false
    }

    fun angle(v: Float4): Float {
        return toDegrees(acos(normalize(this) * normalize(v)))
    }

    override fun toString(): String {
        return "($x, $y, $z, $w)"
    }

    override fun hashCode(): Int {
        var result = x.hashCode()
        result = 31 * result + y.hashCode()
        result = 31 * result + z.hashCode()
        result = 31 * result + w.hashCode()
        return result
    }

    companion object {
        val posX: Float4
            get() = Float4(1f, 0f, 0f, 0f)
        val negX: Float4
            get() = Float4(-1f, 0f, 0f, 0f)
        val posY: Float4
            get() = Float4(0f, 1f, 0f, 0f)
        val negY: Float4
            get() = Float4(0f, -1f, 0f, 0f)
        val posZ: Float4
            get() = Float4(0f, 0f, 1f, 0f)
        val negZ: Float4
            get() = Float4(0f, 0f, -1f, 0f)
        val posW: Float4
            get() = Float4(0f, 0f, 0f, 1f)
        val negW: Float4
            get() = Float4(0f, 0f, 0f, -1f)
    }
}