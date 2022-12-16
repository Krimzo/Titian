package math

import java.awt.Color
import java.io.Serializable

class Float4 : Serializable {
    var x = 0f
    var y = 0f
    var z = 0f
    var w = 0f

    constructor() {
        set(0f)
    }

    constructor(a: Float) {
        set(a)
    }

    constructor(data: FloatArray?) {
        set(data)
    }

    constructor(x: Float, y: Float, z: Float, w: Float) {
        set(x, y, z, w)
    }

    constructor(v: Float2, z: Float, w: Float) {
        set(v, z, w)
    }

    constructor(x: Float, v: Float2, w: Float) {
        set(x, v, w)
    }

    constructor(x: Float, y: Float, v: Float2) {
        set(x, y, v)
    }

    constructor(v1: Float2, v2: Float2) {
        set(v1, v2)
    }

    constructor(v: Float3?, w: Float) {
        set(v, w)
    }

    constructor(x: Float, v: Float3) {
        set(x, v)
    }

    constructor(v: Float4) {
        set(v)
    }

    constructor(v: Int4) {
        set(v)
    }

    constructor(color: Color) {
        set(color)
    }

    // Setters
    operator fun set(x: Float, y: Float, z: Float, w: Float) {
        this.x = x
        this.y = y
        this.z = z
        this.w = w
    }

    fun set(a: Float) {
        set(a, a, a, a)
    }

    fun set(data: FloatArray?) {
        set(data!![0], data[1], data[2], data[3])
    }

    operator fun set(v: Float2, z: Float, w: Float) {
        set(v.x, v.y, z, w)
    }

    operator fun set(x: Float, v: Float2, w: Float) {
        set(x, v.x, v.y, w)
    }

    operator fun set(x: Float, y: Float, v: Float2) {
        set(x, y, v.x, v.y)
    }

    operator fun set(v1: Float2, v2: Float2) {
        set(v1.x, v1.y, v2.x, v2.y)
    }

    operator fun set(v: Float3?, w: Float) {
        set(v!!.x, v.y, v.z, w)
    }

    operator fun set(x: Float, v: Float3) {
        set(x, v.x, v.y, v.z)
    }

    fun set(v: Float4) {
        set(v.x, v.y, v.z, v.w)
    }

    fun set(v: Int4) {
        set(v.x.toFloat(), v.y.toFloat(), v.z.toFloat(), v.w.toFloat())
    }

    fun set(color: Color) {
        set(Float3(color), color.alpha / 255.0f)
    }

    // Getters
    fun xy(): Float2 {
        return Float2(x, y)
    }

    fun xyz(): Float3 {
        return Float3(x, y, z)
    }

    fun array(): FloatArray {
        return floatArrayOf(x, y, z, w)
    }

    fun color(): Color {
        return Color(Math.min(Math.max(x * 255, 0f), 255f).toInt(), Math.min(Math.max(y * 255, 0f), 255f).toInt(), Math.min(Math.max(z * 255, 0f), 255f).toInt(), Math.min(Math.max(w * 255, 0f), 255f).toInt())
    }

    // Math
    fun add(v: Float4): Float4 {
        return Float4(x + v.x, y + v.y, z + v.z, w + v.w)
    }

    fun subtract(v: Float4): Float4 {
        return Float4(x - v.x, y - v.y, z - v.z, w - v.w)
    }

    fun multiply(a: Float): Float4 {
        return Float4(x * a, y * a, z * a, w * a)
    }

    fun multiply(v: Float4): Float4 {
        return Float4(x * v.x, y * v.y, z * v.z, w * v.w)
    }

    fun divide(a: Float): Float4 {
        return multiply(1.0f / a)
    }

    fun divide(v: Float4): Float4 {
        return Float4(x / v.x, y / v.y, z / v.z, w / v.w)
    }

    fun equals(v: Float4): Boolean {
        return x == v.x && y == v.y && z == v.z && w == v.w
    }

    fun negate(): Float4 {
        return multiply(-1f)
    }

    fun length(): Float {
        return Math.sqrt(dot(this).toDouble()).toFloat()
    }

    fun normalize(): Float4 {
        return divide(length())
    }

    fun dot(v: Float4): Float {
        return x * v.x + y * v.y + z * v.z + w * v.w
    }

    fun angle(v: Float4): Float {
        return Math.toDegrees(Math.acos(normalize().dot(v.normalize()).toDouble())).toFloat()
    }

    override fun toString(): String {
        return "($x, $y, $z, $w)"
    }

    companion object {
        val posX: Float4
            get() = Float4(1f, 0f, 0f, 0f)
        val negX: Float4
            get() = Float4(-1f, 0f, 0f, 0f)
        val posY: Float4
            get() = Float4(0f, 1f, 0f, 0f)
        val negY: Float4
            get() = Float4(0f, -1f, 0f, 0f)
        val posZ: Float4
            get() = Float4(0f, 0f, 1f, 0f)
        val negZ: Float4
            get() = Float4(0f, 0f, -1f, 0f)
        val posW: Float4
            get() = Float4(0f, 0f, 0f, 1f)
        val negW: Float4
            get() = Float4(0f, 0f, 0f, -1f)
    }
}