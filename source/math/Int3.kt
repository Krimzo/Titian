package math

import java.awt.Color
import java.io.Serializable

class Int3 : Serializable {
    var x = 0
    var y = 0
    var z = 0

    constructor() {
        set(0)
    }

    constructor(a: Int) {
        set(a)
    }

    constructor(x: Int, y: Int, z: Int) {
        set(x, y, z)
    }

    constructor(v: Int2, z: Int) {
        set(v, z)
    }

    constructor(x: Int, v: Int2) {
        set(x, v)
    }

    constructor(v: Float3) {
        set(v)
    }

    constructor(v: Int3) {
        set(v)
    }

    // Setters
    operator fun set(x: Int, y: Int, z: Int) {
        this.x = x
        this.y = y
        this.z = z
    }

    fun set(a: Int) {
        set(a, a, a)
    }

    fun set(data: IntArray?) {
        set(data!![0], data[1], data[2])
    }

    operator fun set(v: Int2, z: Int) {
        set(v.x, v.y, z)
    }

    operator fun set(x: Int, v: Int2) {
        set(x, v.x, v.y)
    }

    fun set(v: Int3) {
        set(v.x, v.y, v.z)
    }

    fun set(v: Float3) {
        set(v.x.toInt(), v.y.toInt(), v.z.toInt())
    }

    fun set(color: Color) {
        set(color.red, color.green, color.blue)
    }

    // Getters
    fun xy(): Int2 {
        return Int2(x, y)
    }

    fun array(): IntArray {
        return intArrayOf(x, y, z)
    }

    fun color(): Color {
        return Color(x, y, z)
    }

    // Math
    fun add(v: Int3): Int3 {
        return Int3(x + v.x, y + v.y, z + v.z)
    }

    fun subtract(v: Int3): Int3 {
        return Int3(x - v.x, y - v.y, z - v.z)
    }

    fun multiply(a: Int): Int3 {
        return Int3(x * a, y * a, z * a)
    }

    fun multiply(v: Int3): Int3 {
        return Int3(x * v.x, y * v.y, z * v.z)
    }

    fun divide(a: Int): Int3 {
        return Int3(x / a, y / a, z / a)
    }

    fun divide(v: Int3): Int3 {
        return Int3(x / v.x, y / v.y, z / v.z)
    }

    fun equals(v: Int3): Boolean {
        return x == v.x && y == v.y && z == v.z
    }

    fun negate(): Int3 {
        return multiply(-1)
    }

    fun absolute(): Int3 {
        return Int3(Math.abs(x), Math.abs(y), Math.abs(z))
    }

    override fun toString(): String {
        return "($x, $y, $z)"
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