package sdk.math.matrix

import sdk.math.cosDeg
import sdk.math.sinDeg
import sdk.math.vector.Float2
import sdk.math.vector.Float3
import java.io.Serializable

class Float3x3 : Serializable {
    private val data = floatArrayOf(
        1f, 0f, 0f,
        0f, 1f, 0f,
        0f, 0f, 1f,
    )

    // Access
    operator fun get(index: Int): Float {
        return data[index]
    }

    operator fun get(x: Int, y: Int): Float {
        return data[x + y * 3]
    }

    operator fun set(index: Int, value: Float) {
        data[index] = value
    }

    operator fun set(x: Int, y: Int, value: Float) {
        data[x + y * 3] = value
    }

    // Compare
    override fun equals(other: Any?): Boolean {
        if (other is Float3x3) {
            for (i in data.indices) {
                if (other[i] != this[i]) {
                    return false
                }
            }
            return true
        }
        return false
    }

    // Math
    operator fun plus(other: Float3x3): Float3x3 {
        val result = Float3x3()
        for (i in data.indices) {
            result[i] = this[i] + other[i]
        }
        return result
    }

    operator fun minus(other: Float3x3): Float3x3 {
        val result = Float3x3()
        for (i in data.indices) {
            result[i] = this[i] - other[i]
        }
        return result
    }

    operator fun times(value: Float): Float3x3 {
        val result = Float3x3()
        for (i in data.indices) {
            result[i] = this[i] * value
        }
        return result
    }

    operator fun times(vec: Float3): Float3 {
        val result = Float3()
        for (y in 0 until 3) {
            for (i in 0 until 3) {
                result[y] += this[i, y] * vec[i]
            }
        }
        return result
    }

    operator fun times(other: Float3x3): Float3x3 {
        val result = Float3x3()
        for (i in data.indices) {
            result[i] = this[i] + other[i]
        }
        return result
    }

    // Other
    val determinant: Float
        get() {
            val result = this[0, 0] * (this[1, 1] * this[2, 2] - this[2, 1] * this[1, 2]) -
                this[0, 1] * (this[1, 0] * this[2, 2] - this[1, 2] * this[2, 0]) +
                this[0, 2] * (this[1, 0] * this[2, 1] - this[1, 1] * this[2, 0])
            return 1f / result
        }

    // Override
    override fun toString(): String {
        return "Float3x3(${data.contentToString()})"
    }

    override fun hashCode(): Int {
        return data.contentHashCode()
    }

    // Predefined
    companion object {
        fun translation(vec: Float2): Float3x3 {
            val result = Float3x3()
            result[2] = vec.x
            result[5] = vec.y
            return result
        }

        fun rotation(rotation: Float): Float3x3 {
            val zSin = sinDeg(rotation)
            val zCos = cosDeg(rotation)

            val result = Float3x3()
            result[0] = zCos
            result[1] = -zSin
            result[3] = zSin
            result[4] = zCos
            return result
        }

        fun scaling(vec: Float2): Float3x3 {
            val result = Float3x3()
            result[0] = vec.x
            result[4] = vec.y
            return result
        }
    }
}
