package math

import java.io.Serializable

class Mat3() : Matrix(3, 3), Serializable {
    constructor(data: FloatArray?) : this() {
        System.arraycopy(data, 0, this.data, 0, this.data.size)
    }

    constructor(matrix: Matrix?) : this(matrix!!.data)
    constructor(mat: Mat3) : this(mat.data)

    fun add(mat: Mat3): Mat3 {
        return Mat3(super.add(mat))
    }

    fun subtract(mat: Mat3): Mat3 {
        return Mat3(super.subtract(mat))
    }

    fun multiply(vec: Float3): Float3 {
        return Float3(super.multiply(vec.array(), 1, 3).data)
    }

    fun multiply(mat: Mat3?): Mat3 {
        return Mat3(super.multiply(mat))
    }

    override fun absolute(): Mat3 {
        return Mat3(super.absolute())
    }

    override fun negate(): Mat3 {
        return Mat3(super.negate())
    }

    override fun transpose(): Mat3 {
        return Mat3(super.transpose())
    }

    override fun inverse(): Mat3 {
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
            val zSin = Math.sin(Math.toRadians(rotation.toDouble())).toFloat()
            val zCos = Math.cos(Math.toRadians(rotation.toDouble())).toFloat()
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