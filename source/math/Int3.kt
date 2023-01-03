package math

import java.awt.Color
import java.io.Serializable

class Int3 : Serializable {
    var x: Int = 0
    var y: Int = 0
    var z: Int = 0

    constructor(x: Int, y: Int, z: Int) {
        this.x = x
        this.y = y
        this.z = z
    }

    constructor() : this(0, 0, 0)

    constructor(a: Int) : this(a, a, a)

    constructor(data: IntArray) : this(data[0], data[1], data[2])

    constructor(v: Int2, z: Int) : this(v.x, v.y, z)

    constructor(x: Int, v: Int2) : this(x, v.x, v.y)

    constructor(v: Int3): this(v.x, v.y, v.z)

    constructor(v: Float3) : this(v.x.toInt(), v.y.toInt(), v.z.toInt())

    constructor(color: Color) : this(color.red, color.green, color.blue)

    // Getters
    val xy: Int2
        get() = Int2(x, y)

    val array: IntArray
        get() = intArrayOf(x, y, z)

    val color: Color
        get() = Color(x, y, z)

    // Math
    operator fun plus(v: Int3): Int3 {
        return Int3(x + v.x, y + v.y, z + v.z)
    }

    operator fun minus(v: Int3): Int3 {
        return Int3(x - v.x, y - v.y, z - v.z)
    }

    operator fun times(a: Int): Int3 {
        return Int3(x * a, y * a, z * a)
    }

    operator fun div(a: Int): Int3 {
        return Int3(x / a, y / a, z / a)
    }

    operator fun unaryMinus(): Int3 {
        return this * -1
    }

    override fun equals(other: Any?): Boolean {
        if (other is Int3) {
            return x == other.x && y == other.y && z == other.z
        }
        return false
    }

    override fun toString(): String {
        return "($x, $y, $z)"
    }

    override fun hashCode(): Int {
        var result = x
        result = 31 * result + y
        result = 31 * result + z
        return result
    }

    companion object {
        val posX: Int3
            get() = Int3(1, 0, 0)
        val negX: Int3
            get() = Int3(-1, 0, 0)
        val posY: Int3
            get() = Int3(0, 1, 0)
        val negY: Int3
            get() = Int3(0, -1, 0)
        val posZ: Int3
            get() = Int3(0, 0, 1)
        val negZ: Int3
            get() = Int3(0, 0, -1)
    }
}