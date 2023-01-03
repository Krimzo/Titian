package math

import java.io.Serializable
import kotlin.math.cos
import kotlin.math.sin

class Mat3() : Matrix(3, 3), Serializable {
    constructor(data: FloatArray) : this() {
        System.arraycopy(data, 0, this.data, 0, this.data.size)
    }

    constructor(matrix: Matrix) : this(matrix.data)

    operator fun plus(mat: Mat3): Mat3 {
        return Mat3(super.add(mat))
    }

    operator fun minus(mat: Mat3): Mat3 {
        return Mat3(super.subtract(mat))
    }

    operator fun times(value: Float): Mat3 {
        return Mat3(super.multiply(value))
    }

    operator fun times(vec: Float3): Float3 {
        return Float3(super.multiply(vec.array, 1, 3).data)
    }

    operator fun times(mat: Mat3): Mat3 {
        return Mat3(super.multiply(mat))
    }

    operator fun unaryMinus(): Mat3 {
        return Mat3(super.negate())
    }

    fun abs(): Mat3 {
        return Mat3(super.absolute())
    }

    fun tran(): Mat3 {
        return Mat3(super.transpose())
    }

    fun inv(): Mat3 {
        return Mat3(super.inverse())
    }

    companion object {
        fun translation(translation: Float2): Mat3 {
            val result = Mat3()
            result.data[2] = translation.x
            result.data[5] = translation.y
            return result
        }

        fun rotation(rotation: Float): Mat3 {
            val zSin = sin(toRadians(rotation))
            val zCos = cos(toRadians(rotation))
            val result = Mat3()
            result.data[0] = zCos
            result.data[1] = -zSin
            result.data[3] = zSin
            result.data[4] = zCos
            return result
        }

        fun scaling(size: Float2): Mat3 {
            val result = Mat3()
            result.data[0] = size.x
            result.data[4] = size.y
            return result
        }
    }
}