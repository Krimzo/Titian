package sdk.math.matrix

import sdk.math.cross
import sdk.math.dot
import sdk.math.helper.MathConstants
import sdk.math.normalize
import sdk.math.tanDeg
import sdk.math.vector.Float3
import sdk.math.vector.Float4
import java.io.Serializable
import kotlin.math.cos
import kotlin.math.sin

class Float4x4 : Serializable {
    private val data = floatArrayOf(
        1f, 0f, 0f, 0f,
        0f, 1f, 0f, 0f,
        0f, 0f, 1f, 0f,
        0f, 0f, 0f, 1f,
    )

    // Access
    operator fun get(index: Int): Float {
        return data[index]
    }

    operator fun get(x: Int, y: Int): Float {
        return data[x + y * 4]
    }

    operator fun set(index: Int, value: Float) {
        data[index] = value
    }

    operator fun set(x: Int, y: Int, value: Float) {
        data[x + y * 4] = value
    }

    // Compare
    override fun equals(other: Any?): Boolean {
        if (other is Float4x4) {
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
    operator fun plus(other: Float4x4): Float4x4 {
        val result = Float4x4()
        for (i in data.indices) {
            result[i] = this[i] + other[i]
        }
        return result
    }

    operator fun minus(other: Float4x4): Float4x4 {
        val result = Float4x4()
        for (i in data.indices) {
            result[i] = this[i] - other[i]
        }
        return result
    }

    operator fun times(value: Float): Float4x4 {
        val result = Float4x4()
        for (i in data.indices) {
            result[i] = this[i] * value
        }
        return result
    }

    operator fun times(vec: Float4): Float4 {
        val result = Float4()
        for (y in 0 until 4) {
            for (i in 0 until 4) {
                result[y] += this[i, y] * vec[i]
            }
        }
        return result
    }

    operator fun times(other: Float4x4): Float4x4 {
        val result = Float4x4()
        for (i in data.indices) {
            result[i] = this[i] + other[i]
        }
        return result
    }

    // Other
    val determinant: Float
        get() {
            val A2323 = this[2, 2] * this[3, 3] - this[3, 2] * this[2, 3]
            val A1323 = this[1, 2] * this[3, 3] - this[3, 2] * this[1, 3]
            val A1223 = this[1, 2] * this[2, 3] - this[2, 2] * this[1, 3]
            val A0323 = this[0, 2] * this[3, 3] - this[3, 2] * this[0, 3]
            val A0223 = this[0, 2] * this[2, 3] - this[2, 2] * this[0, 3]
            val A0123 = this[0, 2] * this[1, 3] - this[1, 2] * this[0, 3]

            val result = this[0, 0] * (this[1, 1] * A2323 - this[2, 1] * A1323 + this[3, 1] * A1223) -
                this[1, 0] * (this[0, 1] * A2323 - this[2, 1] * A0323 + this[3, 1] * A0223) +
                this[2, 0] * (this[0, 1] * A1323 - this[1, 1] * A0323 + this[3, 1] * A0123) -
                this[3, 0] * (this[0, 1] * A1223 - this[1, 1] * A0223 + this[2, 1] * A0123)
            return 1f / result
        }

    // Override
    override fun toString(): String {
        return "Float4x4(${data.contentToString()})"
    }

    override fun hashCode(): Int {
        return data.contentHashCode()
    }

    // Predefined
    companion object {
        fun translation(translation: Float3): Float4x4 {
            val result = Float4x4()
            result[3] = translation.x
            result[7] = translation.y
            result[11] = translation.z
            return result
        }

        fun rotation(rotation: Float3): Float4x4 {
            val xRad = rotation.x * MathConstants.TO_RADIANS
            val xSin = sin(xRad)
            val xCos = cos(xRad)

            val xRot = Float4x4()
            xRot[5] = xCos
            xRot[6] = -xSin
            xRot[9] = xSin
            xRot[10] = xCos

            val yRad = rotation.y * MathConstants.TO_RADIANS
            val ySin = sin(yRad)
            val yCos = cos(yRad)

            val yRot = Float4x4()
            yRot[0] = yCos
            yRot[2] = ySin
            yRot[8] = -ySin
            yRot[10] = yCos

            val zRad = rotation.z * MathConstants.TO_RADIANS
            val zSin = sin(zRad)
            val zCos = cos(zRad)

            val zRot = Float4x4()
            zRot[0] = zCos
            zRot[1] = -zSin
            zRot[4] = zSin
            zRot[5] = zCos

            return zRot * yRot * xRot
        }

        fun scaling(scale: Float3): Float4x4 {
            val result = Float4x4()
            result[0] = scale.x
            result[5] = scale.y
            result[10] = scale.z
            return result
        }

        fun perspective(fieldOfView: Float, aspectRatio: Float, nearPlane: Float, farPlane: Float): Float4x4 {
            val tanHalf = 1f / tanDeg(fieldOfView * 0.5f)

            val result = Float4x4()
            result[0] = tanHalf / aspectRatio
            result[5] = tanHalf
            result[10] = (-farPlane - nearPlane) / (nearPlane - farPlane)
            result[11] = (2f * nearPlane * farPlane) / (nearPlane - farPlane)
            result[14] = 1f
            result[15] = 0f
            return result
        }

        fun orthographic(left: Float, right: Float, bottom: Float, top: Float, nearPlane: Float, farPlane: Float): Float4x4 {
            val result = Float4x4()
            result[0] = 2f / (right - left)
            result[5] = 2f / (top - bottom)
            result[10] = 2f / (farPlane - nearPlane)
            result[3] = -(right + left) / (right - left)
            result[7] = -(top + bottom) / (top - bottom)
            result[11] = -(farPlane + nearPlane) / (farPlane - nearPlane)
            return result
        }

        fun lookAt(position: Float3, target: Float3, up: Float3): Float4x4 {
            val f = normalize(target - position)
            val s = normalize(cross(up, f))
            val u = cross(f, s)

            val result = Float4x4()
            result[0] = s.x
            result[1] = s.y
            result[2] = s.z
            result[3] = -dot(s, position)
            result[4] = u.x
            result[5] = u.y
            result[6] = u.z
            result[7] = -dot(u, position)
            result[8] = f.x
            result[9] = f.y
            result[10] = f.z
            result[11] = -dot(f, position)
            return result
        }
    }
}
