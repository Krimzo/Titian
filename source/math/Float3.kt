package math

import java.awt.Color
import java.io.Serializable
import kotlin.math.acos
import kotlin.math.cos
import kotlin.math.sin
import kotlin.math.sqrt

class Float3 : Serializable {
    var x: Float = 0f
    var y: Float = 0f
    var z: Float = 0f

    constructor(x: Float, y: Float, z: Float) {
        this.x = x
        this.y = y
        this.z = z
    }

    constructor() : this(0f, 0f, 0f)

    constructor(a: Float) : this(a, a, a)

    constructor(data: FloatArray) : this(data[0], data[1], data[2])

    constructor(v: Float2, z: Float) : this(v.x, v.y, z)

    constructor(x: Float, v: Float2) : this(x, v.x, v.y)

    constructor(v: Float3) : this(v.x, v.y, v.z)

    constructor(v: Int3) : this(v.x.toFloat(), v.y.toFloat(), v.z.toFloat())

    constructor(color: Color) {
        val conv = 1f / 255f
        x = color.red * conv
        y = color.green * conv
        z = color.blue * conv
    }

    // Getters
    val length: Float
        get() = sqrt(this * this)

    val xy: Float2
        get() = Float2(x, y)

    val array: FloatArray
        get() = floatArrayOf(x, y, z)

    val color: Color
        get() = Color(
            (x * 255).toInt().coerceAtLeast(0).coerceAtMost(255),
            (y * 255).toInt().coerceAtLeast(0).coerceAtMost(255),
            (z * 255).toInt().coerceAtLeast(0).coerceAtMost(255)
        )

    // Math
    operator fun plus(v: Float3): Float3 {
        return Float3(x + v.x, y + v.y, z + v.z)
    }

    operator fun minus(v: Float3): Float3 {
        return Float3(x - v.x, y - v.y, z - v.z)
    }

    operator fun times(a: Float): Float3 {
        return Float3(x * a, y * a, z * a)
    }

    operator fun times(v: Float3): Float {
        return x * v.x + y * v.y + z * v.z
    }

    infix fun x(v: Float3): Float3 {
        return Float3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x)
    }

    operator fun div(a: Float): Float3 {
        return this * (1f / a)
    }

    operator fun unaryMinus(): Float3 {
        return this * -1f
    }

    override fun equals(other: Any?): Boolean {
        if (other is Float3) {
            return x == other.x && y == other.y && z == other.z
        }
        return false
    }

    fun angle(v: Float3): Float {
        return toDegrees(acos(normalize(this) * normalize(v)))
    }

    fun rotate(angle: Float, axis: Float3): Float3 {
        val angleSin = sin(toRadians(angle * 0.5f))
        val angleCos = cos(toRadians(angle * 0.5f))
        val qx = axis.x * angleSin
        val qy = axis.y * angleSin
        val qz = axis.z * angleSin
        val x2 = qx * qx
        val y2 = qy * qy
        val z2 = qz * qz
        val w2 = angleCos * angleCos
        val xy = qx * qy
        val xz = qx * qz
        val yz = qy * qz
        val xw = qx * angleCos
        val yw = qy * angleCos
        val zw = qz * angleCos

        val temp = Float3(0f)
        temp.x = (w2 + x2 - z2 - y2) * x + (-zw + xy - zw + xy) * y + (yw + xz + xz + yw) * z
        temp.y = (xy + zw + zw + xy) * x + (y2 - z2 + w2 - x2) * y + (yz + yz - xw - xw) * z
        temp.z = (xz - yw + xz - yw) * x + (yz + yz + xw + xw) * y + (z2 - y2 - x2 + w2) * z
        return temp
    }

    fun reflect(v: Float3): Float3 {
        val normal = normalize(v)
        return this - (normal * (this * normal * 2f))
    }

    override fun toString(): String {
        return "($x, $y, $z)"
    }

    override fun hashCode(): Int {
        var result = x.hashCode()
        result = 31 * result + y.hashCode()
        result = 31 * result + z.hashCode()
        return result
    }

    companion object {
        val posX: Float3
            get() = Float3(1f, 0f, 0f)
        val negX: Float3
            get() = Float3(-1f, 0f, 0f)
        val posY: Float3
            get() = Float3(0f, 1f, 0f)
        val negY: Float3
            get() = Float3(0f, -1f, 0f)
        val posZ: Float3
            get() = Float3(0f, 0f, 1f)
        val negZ: Float3
            get() = Float3(0f, 0f, -1f)
    }
}