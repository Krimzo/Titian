package sdk.math.imaginary

import sdk.math.cosDeg
import sdk.math.normalize
import sdk.math.sinDeg
import sdk.math.vector.Float3
import sdk.math.vector.Float4
import kotlin.math.sqrt

class Quaternion {
    var w: Float
    var x: Float
    var y: Float
    var z: Float

    // Construct
    constructor() : this(1f, 0f, 0f, 0f)

    constructor(x: Float, y: Float, z: Float) : this(0f, x, y, z)

    constructor(w: Float, x: Float, y: Float, z: Float) {
        this.w = w
        this.x = x
        this.y = y
        this.z = z
    }

    constructor(axis: Float3, angle: Float) {
        w = cosDeg(angle * 0.5f)
        (normalize(axis) * sinDeg(angle * 0.5f)).let {
            x = it.x
            y = it.y
            z = it.z
        }
    }

    // Cast
    val float3: Float3
        get() = Float3(x, y, z)

    val float4: Float4
        get() = Float4(x, y, z, w)

    // Compare
    override fun equals(other: Any?): Boolean {
        if (other is Quaternion) {
            return (w == other.w && x == other.x && y == other.y && z == other.z)
        }
        return false
    }

    // Math
    operator fun plus(other: Quaternion): Quaternion {
        return Quaternion(w + other.w, x + other.x, y + other.y, z + other.z)
    }

    operator fun minus(other: Quaternion): Quaternion {
        return Quaternion(w - other.w, x - other.x, y - other.y, z - other.z)
    }

    operator fun times(value: Float): Quaternion {
        return Quaternion(w * value, x * value, y * value, z * value)
    }

    operator fun times(other: Quaternion): Quaternion {
        return Quaternion(
            w * other.w - x * other.x - y * other.y - z * other.z,
            w * other.x + x * other.w + y * other.z - z * other.y,
            w * other.y - x * other.z + y * other.w + z * other.x,
            w * other.z + x * other.y - y * other.x + z * other.w,
        )
    }

    // Other
    operator fun unaryMinus(): Quaternion {
        return this * -1f
    }

    val length: Float
        get() = sqrt(w * w + x * x + y * y + z * z)

    // Override
    override fun toString(): String {
        return "Quaternion($w + ${x}i + ${y}j + ${z}k)"
    }

    override fun hashCode(): Int {
        var result = w.hashCode()
        result = 31 * result + x.hashCode()
        result = 31 * result + y.hashCode()
        result = 31 * result + z.hashCode()
        return result
    }
}
