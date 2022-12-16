package math

import java.awt.Color
import java.io.Serializable

class Int4 : Serializable {
    var x = 0
    var y = 0
    var z = 0
    var w = 0

    constructor() {
        set(0)
    }

    constructor(a: Int) {
        set(a)
    }

    constructor(data: IntArray?) {
        set(data)
    }

    constructor(x: Int, y: Int, z: Int, w: Int) {
        set(x, y, z, w)
    }

    constructor(v: Int2, z: Int, w: Int) {
        set(v, z, w)
    }

    constructor(x: Int, v: Int2, w: Int) {
        set(x, v, w)
    }

    constructor(x: Int, y: Int, v: Int2) {
        set(x, y, v)
    }

    constructor(v1: Int2, v2: Int2) {
        set(v1, v2)
    }

    constructor(v: Int3, w: Int) {
        set(v, w)
    }

    constructor(x: Int, v: Int3) {
        set(x, v)
    }

    constructor(v: Int4) {
        set(v)
    }

    constructor(v: Float4) {
        set(v)
    }

    constructor(color: Color) {
        set(color)
    }

    // Setters
    operator fun set(x: Int, y: Int, z: Int, w: Int) {
        this.x = x
        this.y = y
        this.z = z
        this.w = w
    }

    fun set(a: Int) {
        set(a, a, a, a)
    }

    fun set(data: IntArray?) {
        set(data!![0], data[1], data[2], data[3])
    }

    operator fun set(v: Int2, z: Int, w: Int) {
        set(v.x, v.y, z, w)
    }

    operator fun set(x: Int, v: Int2, w: Int) {
        set(x, v.x, v.y, w)
    }

    operator fun set(x: Int, y: Int, v: Int2) {
        set(x, y, v.x, v.y)
    }

    operator fun set(v1: Int2, v2: Int2) {
        set(v1.x, v1.y, v2.x, v2.y)
    }

    operator fun set(v: Int3, w: Int) {
        set(v.x, v.y, v.z, w)
    }

    operator fun set(x: Int, v: Int3) {
        set(x, v.x, v.y, v.z)
    }

    fun set(v: Int4) {
        set(v.x, v.y, v.z, v.w)
    }

    fun set(v: Float4) {
        set(v.x.toInt(), v.y.toInt(), v.z.toInt(), v.w.toInt())
    }

    fun set(color: Color) {
        set(color.red, color.green, color.blue, color.alpha)
    }

    // Getters
    fun xy(): Int2 {
        return Int2(x, y)
    }

    fun xyz(): Int3 {
        return Int3(x, y, z)
    }

    fun array(): IntArray {
        return intArrayOf(x, y, z, w)
    }

    fun color(): Color {
        return Color(x, y, z, w)
    }

    // Math
    fun add(v: Int4): Int4 {
        return Int4(x + v.x, y + v.y, z + v.z, w + v.w)
    }

    fun subtract(v: Int4): Int4 {
        return Int4(x - v.x, y - v.y, z - v.z, w - v.w)
    }

    fun multiply(a: Int): Int4 {
        return Int4(x * a, y * a, z * a, w * a)
    }

    fun multiply(v: Int4): Int4 {
        return Int4(x * v.x, y * v.y, z * v.z, w * v.w)
    }

    fun divide(a: Int): Int4 {
        return Int4(x / a, y / a, z / a, w / a)
    }

    fun divide(v: Int4): Int4 {
        return Int4(x / v.x, y / v.y, z / v.z, w / v.w)
    }

    fun equals(v: Int4): Boolean {
        return x == v.x && y == v.y && z == v.z && w == v.w
    }

    fun negate(): Int4 {
        return multiply(-1)
    }

    fun absolute(): Int4 {
        return Int4(Math.abs(x), Math.abs(y), Math.abs(z), Math.abs(w))
    }

    override fun toString(): String {
        return "($x, $y, $z, $w)"
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