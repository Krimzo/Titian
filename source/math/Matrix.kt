package math

import utility.helper.StringHelper
import java.io.Serializable

open class Matrix(val width: Int, val height: Int) : Serializable {
    val data: FloatArray = FloatArray(width * height)

    init {
        if (isSquare) {
            var i = 0
            while (i < width * height) {
                data[i] = 1f
                i += width + 1
            }
        }
    }

    operator fun get(i: Int): Float {
        return data[i]
    }

    operator fun set(i: Int, value: Float) {
        data[i] = value
    }

    operator fun get(x: Int, y: Int): Float {
        return get(y * width + x)
    }

    operator fun set(x: Int, y: Int, value: Float) {
        set(y * width + x, value)
    }

    val isSquare: Boolean
        get() = width == height

    fun sizeEquals(width: Int, height: Int): Boolean {
        return this.width == width && this.height == height
    }

    fun sizeEquals(matrix: Matrix): Boolean {
        return sizeEquals(matrix.width, matrix.height)
    }

    fun add(matrix: Matrix): Matrix {
        val result = Matrix(width, height)
        if (sizeEquals(matrix)) {
            for (i in 0 until width * height) {
                result[i] = get(i) + matrix[i]
            }
        }
        return result
    }

    fun subtract(matrix: Matrix): Matrix {
        val result = Matrix(width, height)
        if (sizeEquals(matrix)) {
            for (i in 0 until width * height) {
                result[i] = get(i) - matrix[i]
            }
        }
        return result
    }

    fun multiply(value: Float): Matrix {
        val result = Matrix(width, height)
        for (i in 0 until width * height) {
            result[i] = get(i) * value
        }
        return result
    }

    fun multiply(data: FloatArray?, W: Int, H: Int): Matrix {
        val result = Matrix(W, height)
        if (width == H) {
            for (y in 0 until height) {
                for (x in 0 until W) {
                    result[x, y] = 0f
                    for (i in 0 until width) {
                        result[x, y] = result[x, y] + get(i, y) * data!![i * W + x]
                    }
                }
            }
        }
        return result
    }

    fun multiply(matrix: Matrix?): Matrix {
        return multiply(matrix!!.data, matrix.width, matrix.height)
    }

    fun equals(matrix: Matrix): Boolean {
        if (!sizeEquals(matrix)) {
            return false
        }
        for (i in 0 until width * height) {
            if (get(i) != matrix[i]) {
                return false
            }
        }
        return true
    }

    open fun absolute(): Matrix {
        val result = Matrix(width, height)
        for (i in 0 until width * height) {
            result[i] = Math.abs(get(i))
        }
        return result
    }

    open fun negate(): Matrix {
        return multiply(-1f)
    }

    open fun transpose(): Matrix {
        val result = Matrix(width, height)
        for (y in 0 until height) {
            for (x in 0 until width) {
                result[x, y] = get(y, x)
            }
        }
        return result
    }

    fun cofactor(index: Int): Matrix {
        return cofactor(index % width, index / height)
    }

    fun cofactor(X: Int, Y: Int): Matrix {
        val result = Matrix(width - 1, height - 1)
        if (isSquare) {
            var counter = 0
            for (y in 0 until height) {
                for (x in 0 until width) {
                    if (x != X && y != Y) {
                        result[counter++] = get(x, y)
                    }
                }
            }
        }
        return result
    }

    fun cofactor(): Matrix {
        val result = Matrix(width, height)
        if (isSquare) {
            for (y in 0 until height) {
                for (x in 0 until width) {
                    result[x, y] = (if ((y + x + 2) % 2 != 0) -1 else 1) * cofactor(y * width + x).determinant()
                }
            }
        }
        return result
    }

    fun determinant(): Float {
        if (isSquare) {
            if (width == 2) {
                return get(0) * get(3) - get(1) * get(2)
            }
            if (width > 2) {
                var multi = -1
                var result = 0f
                for (i in 0 until width) {
                    result += -1.let { multi *= it; multi } * get(i) * cofactor(i).determinant()
                }
                return result
            }
        }
        return 0f
    }

    fun adjoint(): Matrix {
        return if (isSquare) {
            cofactor().transpose()
        } else Matrix(width, height)
    }

    open fun inverse(): Matrix {
        return if (isSquare) {
            adjoint().multiply(1 / determinant())
        } else Matrix(width, height)
    }

    override fun toString(): String {
        val maxLengths = IntArray(width)
        val outputData = arrayOfNulls<String>(width * height)
        for (x in 0 until width) {
            for (y in 0 until height) {
                outputData[y * width + x] = String.format("%.2f", get(y * width + x))
                maxLengths[x] = Math.max(maxLengths[x], outputData[y * width + x]!!.length)
            }
        }
        val stream = StringBuilder()
        for (y in 0 until height) {
            stream.append("[")
            for (x in 0 until width - 1) {
                stream.append(StringHelper.spaces(maxLengths[x] - outputData[y * width + x]!!.length))
                stream.append(outputData[y * width + x])
                stream.append(" ")
            }
            val lastLine = outputData[y * width + (width - 1)]
            stream.append(StringHelper.spaces(maxLengths[width - 1] - lastLine!!.length))
            stream.append(lastLine)
            stream.append(if (y == height - 1) "]" else "]\n")
        }
        return stream.toString()
    }
}