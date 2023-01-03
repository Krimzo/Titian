package math

import java.awt.Color
import java.io.Serializable

class Int4 : Serializable {
    var x: Int = 0
    var y: Int = 0
    var z: Int = 0
    var w: Int = 0

    constructor(x: Int, y: Int, z: Int, w: Int) {
        this.x = x
        this.y = y
        this.z = z
        this.w = w
    }

    constructor() : this(0, 0, 0, 0)

    constructor(a: Int) : this(a, a, a, a)

    constructor(data: IntArray) : this(data[0], data[1], data[2], data[3])

    constructor(v: Int2, z: Int, w: Int) : this(v.x, v.y, z, w)

    constructor(x: Int, v: Int2, w: Int) : this(x, v.x, v.y, w)

    constructor(x: Int, y: Int, v: Int2) : this(x, y, v.x, v.y)

    constructor(v1: Int2, v2: Int2) : this(v1.x, v1.y, v2.x, v2.y)

    constructor(v: Int3, w: Int) : this(v.x, v.y, v.z, w)

    constructor(x: Int, v: Int3) : this(x, v.x, v.y, v.z)

    constructor(v: Int4) : this(v.x, v.y, v.z, v.w)

    constructor(v: Float4) : this(v.x.toInt(), v.y.toInt(), v.z.toInt(), v.w.toInt())

    constructor(color: Color) : this(color.red, color.green, color.blue, color.alpha)

    // Getters
    val xy: Int2
        get() = Int2(x, y)

    val xyz: Int3
        get() = Int3(x, y, z)

    val array: IntArray
        get() = intArrayOf(x, y, z, w)

    val color: Color
        get() = Color(x, y, z, w)

    // Math
    operator fun plus(v: Int4): Int4 {
        return Int4(x + v.x, y + v.y, z + v.z, w + v.w)
    }

    operator fun minus(v: Int4): Int4 {
        return Int4(x - v.x, y - v.y, z - v.z, w - v.w)
    }

    operator fun times(a: Int): Int4 {
        return Int4(x * a, y * a, z * a, w * a)
    }

    operator fun div(a: Int): Int4 {
        return Int4(x / a, y / a, z / a, w / a)
    }

    operator fun unaryMinus(): Int4 {
        return this * -1
    }

    override fun equals(other: Any?): Boolean {
        if (other is Int4) {
            return x == other.x && y == other.y && z == other.z && w == other.w
        }
        return false
    }

    override fun toString(): String {
        return "($x, $y, $z, $w)"
    }

    override fun hashCode(): Int {
        var result = x
        result = 31 * result + y
        result = 31 * result + z
        result = 31 * result + w
        return result
    }

    companion object {
        val posX: Int4
            get() = Int4(1, 0, 0, 0)
        val negX: Int4
            get() = Int4(-1, 0, 0, 0)
        val posY: Int4
            get() = Int4(0, 1, 0, 0)
        val negY: Int4
            get() = Int4(0, -1, 0, 0)
        val posZ: Int4
            get() = Int4(0, 0, 1, 0)
        val negZ: Int4
            get() = Int4(0, 0, -1, 0)
        val posW: Int4
            get() = Int4(0, 0, 0, 1)
        val negW: Int4
            get() = Int4(0, 0, 0, -1)
    }
}