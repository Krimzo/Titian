package math

import java.io.Serializable

class Int2 : Serializable {
    var x = 0
    var y = 0

    constructor(x: Int, y: Int) {
        this.x = x
        this.y = y
    }

    constructor() : this(0, 0)

    constructor(a: Int) : this(a, a)

    constructor(data: IntArray) : this(data[0], data[1])

    constructor(v: Int2) : this(v.x, v.y)

    constructor(v: Float2) : this(v.x.toInt(), v.y.toInt())

    // Getters
    val array: IntArray
        get() = intArrayOf(x, y)

    // Math
    operator fun plus(v: Int2): Int2 {
        return Int2(x + v.x, y + v.y)
    }

    operator fun minus(v: Int2): Int2 {
        return Int2(x - v.x, y - v.y)
    }

    operator fun times(a: Int): Int2 {
        return Int2(x * a, y * a)
    }

    operator fun div(a: Int): Int2 {
        return Int2(x / a, y / a)
    }

    override fun equals(other: Any?): Boolean {
        if (other is Int2) {
            return x == other.x && y == other.y
        }
        return false
    }

    operator fun unaryMinus(): Int2 {
        return this * -1
    }

    override fun toString(): String {
        return "($x, $y)"
    }

    override fun hashCode(): Int {
        var result = x
        result = 31 * result + y
        return result
    }

    companion object {
        val posX: Int2
            get() = Int2(1, 0)
        val negX: Int2
            get() = Int2(-1, 0)
        val posY: Int2
            get() = Int2(0, 1)
        val negY: Int2
            get() = Int2(0, -1)
    }
}