package sdk.math.imaginary

import sdk.math.cosDeg
import sdk.math.sinDeg
import sdk.math.vector.Float2
import kotlin.math.sqrt

class Complex {
    var r: Float
    var i: Float

    // Construct
    constructor() : this(0f, 0f)

    constructor(r: Float, i: Float) {
        this.r = r
        this.i = i
    }

    constructor(angle: Float) : this(cosDeg(angle), sinDeg(angle))

    // Cast
    val float2: Float2
        get() = Float2(r, i)

    // Compare
    override fun equals(other: Any?): Boolean {
        if (other is Complex) {
            return (r == other.r && i == other.i)
        }
        return false
    }

    // Math
    operator fun plus(other: Complex): Complex {
        return Complex(r + other.r, i + other.i)
    }

    operator fun minus(other: Complex): Complex {
        return Complex(r - other.r, i - other.i)
    }

    operator fun times(value: Float): Complex {
        return Complex(r * value, i * value)
    }

    operator fun times(other: Complex): Complex {
        return Complex(r * other.r - i * other.i, r * other.i + i * other.r)
    }

    // Other
    operator fun unaryMinus(): Complex {
        return this * -1f
    }

    val length: Float
        get() = sqrt(r * r + i * i)

    // Override
    override fun toString(): String {
        return "Complex($r + ${i}i)"
    }

    override fun hashCode(): Int {
        var result = r.hashCode()
        result = 31 * result + i.hashCode()
        return result
    }
}
