package math

import java.io.Serializable

class Mat4() : Matrix(4, 4), Serializable {
    constructor(data: FloatArray?) : this() {
        System.arraycopy(data, 0, this.data, 0, this.data.size)
    }

    constructor(matrix: Matrix?) : this(matrix!!.data)
    constructor(mat: Mat4) : this(mat.data)

    fun add(mat: Mat4): Mat4 {
        return Mat4(super.add(mat))
    }

    fun subtract(mat: Mat4): Mat4 {
        return Mat4(super.subtract(mat))
    }

    fun multiply(vec: Float4): Float4 {
        return Float4(super.multiply(vec.array(), 1, 4).data)
    }

    fun multiply(mat: Mat4?): Mat4 {
        return Mat4(super.multiply(mat))
    }

    override fun absolute(): Mat4 {
        return Mat4(super.absolute())
    }

    override fun negate(): Mat4 {
        return Mat4(super.negate())
    }

    override fun transpose(): Mat4 {
        return Mat4(super.transpose())
    }

    override fun inverse(): Mat4 {
        return Mat4(super.inverse())
    }

    companion object {
        fun translation(translation: Float3): Mat4 {
            val result = Mat4()
            result.data[3] = translation.x
            result.data[7] = translation.y
            result.data[11] = translation.z
            return result
        }

        fun rotation(rotation: Float3): Mat4 {
            val xSin = Math.sin(Math.toRadians(rotation.x.toDouble())).toFloat()
            val xCos = Math.cos(Math.toRadians(rotation.x.toDouble())).toFloat()
            val xRot = Mat4()
            xRot.data[5] = xCos
            xRot.data[6] = -xSin
            xRot.data[9] = xSin
            xRot.data[10] = xCos
            val ySin = Math.sin(Math.toRadians(rotation.y.toDouble())).toFloat()
            val yCos = Math.cos(Math.toRadians(rotation.y.toDouble())).toFloat()
            val yRot = Mat4()
            yRot.data[0] = yCos
            yRot.data[2] = ySin
            yRot.data[8] = -ySin
            yRot.data[10] = yCos
            val zSin = Math.sin(Math.toRadians(rotation.z.toDouble())).toFloat()
            val zCos = Math.cos(Math.toRadians(rotation.z.toDouble())).toFloat()
            val zRot = Mat4()
            zRot.data[0] = zCos
            zRot.data[1] = -zSin
            zRot.data[4] = zSin
            zRot.data[5] = zCos
            return zRot.multiply(yRot).multiply(xRot)
        }

        fun scaling(size: Float3): Mat4 {
            val result = Mat4()
            result.data[0] = size.x
            result.data[5] = size.y
            result.data[10] = size.z
            return result
        }

        fun perspective(fov: Float, aspect: Float, zNear: Float, zFar: Float): Mat4 {
            val xScale = 1.0f / Math.tan(Math.toRadians((fov * 0.5f).toDouble())).toFloat()
            val yScale = xScale * aspect
            val result = Mat4()
            result.data[0] = xScale
            result.data[5] = yScale
            result.data[10] = zFar / (zNear - zFar)
            result.data[11] = zNear * zFar / (zNear - zFar)
            result.data[14] = -1f
            result.data[15] = 0f
            return result
        }

        fun orthographic(width: Float, height: Float, zNear: Float, zFar: Float): Mat4 {
            val result = Mat4()
            result.data[0] = 2 / width
            result.data[5] = 2 / height
            result.data[10] = 1 / (zNear - zFar)
            result.data[11] = zNear / (zNear - zFar)
            return result
        }

        fun lookAt(pos: Float3?, tar: Float3?, up: Float3?): Mat4 {
            val f = pos!!.subtract(tar).normalize()
            val s = up!!.cross(f).normalize()
            val u = f!!.cross(s)
            val result = Mat4()
            result.data[0] = s!!.x
            result.data[1] = s.y
            result.data[2] = s.z
            result.data[3] = -s.dot(pos)
            result.data[4] = u!!.x
            result.data[5] = u.y
            result.data[6] = u.z
            result.data[7] = -u.dot(pos)
            result.data[8] = f.x
            result.data[9] = f.y
            result.data[10] = f.z
            result.data[11] = -f.dot(pos)
            return result
        }
    }
}