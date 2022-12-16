package math

import java.io.Serializable

class Int2 : Serializable {
    var x = 0
    var y = 0

    constructor() {
        set(0)
    }

    constructor(a: Int) {
        set(a)
    }

    constructor(data: IntArray?) {
        set(data)
    }

    constructor(x: Int, y: Int) {
        set(x, y)
    }

    constructor(v: Int2?) {
        set(v)
    }

    constructor(v: Float2) {
        set(v)
    }

    // Setters
    operator fun set(x: Int, y: Int) {
        this.x = x
        this.y = y
    }

    fun set(a: Int) {
        set(a, a)
    }

    fun set(data: IntArray?) {
        set(data!![0], data[1])
    }

    fun set(v: Int2?) {
        set(v!!.x, v.y)
    }

    fun set(v: Float2) {
        set(v.x.toInt(), v.y.toInt())
    }

    // Getters
    fun array(): IntArray {
        return intArrayOf(x, y)
    }

    // Math
    fun add(v: Int2): Int2 {
        return Int2(x + v.x, y + v.y)
    }

    fun subtract(v: Int2?): Int2 {
        return Int2(x - v!!.x, y - v.y)
    }

    fun multiply(a: Int): Int2 {
        return Int2(x * a, y * a)
    }

    fun multiply(v: Int2): Int2 {
        return Int2(x * v.x, y * v.y)
    }

    fun divide(a: Int): Int2 {
        return Int2(x / a, y / a)
    }

    fun divide(v: Int2): Int2 {
        return Int2(x / v.x, y / v.y)
    }

    fun equals(v: Int2): Boolean {
        return x == v.x && y == v.y
    }

    fun negate(): Int2 {
        return multiply(-1)
    }

    fun absolute(): Int2 {
        return Int2(Math.abs(x), Math.abs(y))
    }

    override fun toString(): String {
        return "($x, $y)"
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