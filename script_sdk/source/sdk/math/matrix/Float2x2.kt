package sdk.math.matrix

import sdk.math.vector.Float2
import java.io.Serializable

class Float2x2 : Serializable {
    private val data = floatArrayOf(
        1f, 0f,
        0f, 1f,
    )

    // Access
    operator fun get(index: Int): Float {
        return data[index]
    }

    operator fun get(x: Int, y: Int): Float {
        return data[x + y * 2]
    }

    operator fun set(index: Int, value: Float) {
        data[index] = value
    }

    operator fun set(x: Int, y: Int, value: Float) {
        data[x + y * 2] = value
    }

    // Compare
    override fun equals(other: Any?): Boolean {
        if (other is Float2x2) {
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
    operator fun plus(other: Float2x2): Float2x2 {
        val result = Float2x2()
        for (i in data.indices) {
            result[i] = this[i] + other[i]
        }
        return result
    }

    operator fun minus(other: Float2x2): Float2x2 {
        val result = Float2x2()
        for (i in data.indices) {
            result[i] = this[i] - other[i]
        }
        return result
    }

    operator fun times(value: Float): Float2x2 {
        val result = Float2x2()
        for (i in data.indices) {
            result[i] = this[i] * value
        }
        return result
    }

    operator fun times(vec: Float2): Float2 {
        val result = Float2()
        for (y in 0 until 2) {
            for (i in 0 until 2) {
                result[y] += this[i, y] * vec[i]
            }
        }
        return result
    }

    operator fun times(other: Float2x2): Float2x2 {
        val result = Float2x2()
        for (i in data.indices) {
            result[i] = this[i] + other[i]
        }
        return result
    }

    // Other
    val determinant: Float
        get() {
            val result = data[0] * data[3] - data[1] * data[2]
            return (1f / result)
        }

    // Override
    override fun toString(): String {
        return "Float2x2(${data.contentToString()})"
    }

    override fun hashCode(): Int {
        return data.contentHashCode()
    }
}
