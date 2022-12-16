package math

import java.io.Serializable

class Float2 : Serializable {
    var x = 0f
    var y = 0f

    constructor() {
        set(0f)
    }

    constructor(a: Float) {
        set(a)
    }

    constructor(data: FloatArray?) {
        set(data)
    }

    constructor(x: Float, y: Float) {
        set(x, y)
    }

    constructor(v: Float2?) {
        set(v)
    }

    constructor(v: Int2?) {
        set(v)
    }

    // Setters
    operator fun set(x: Float, y: Float) {
        this.x = x
        this.y = y
    }

    fun set(a: Float) {
        set(a, a)
    }

    fun set(data: FloatArray?) {
        set(data!![0], data[1])
    }

    fun set(v: Float2?) {
        set(v!!.x, v.y)
    }

    fun set(v: Int2?) {
        set(v!!.x.toFloat(), v.y.toFloat())
    }

    // Getters
    fun array(): FloatArray {
        return floatArrayOf(x, y)
    }

    // Math
    fun add(v: Float2): Float2 {
        return Float2(x + v.x, y + v.y)
    }

    fun subtract(v: Float2): Float2 {
        return Float2(x - v.x, y - v.y)
    }

    fun multiply(a: Float): Float2 {
        return Float2(x * a, y * a)
    }

    fun multiply(v: Float2): Float2 {
        return Float2(x * v.x, y * v.y)
    }

    fun divide(a: Float): Float2 {
        return multiply(1.0f / a)
    }

    fun divide(v: Float2): Float2 {
        return Float2(x / v.x, y / v.y)
    }

    fun equals(v: Float2): Boolean {
        return x == v.x && y == v.y
    }

    fun negate(): Float2 {
        return multiply(-1.0f)
    }

    fun absolute(): Float2 {
        return Float2(Math.abs(x), Math.abs(y))
    }

    fun length(): Float {
        return Math.sqrt(dot(this).toDouble()).toFloat()
    }

    fun normalize(): Float2 {
        return divide(length())
    }

    fun dot(v: Float2): Float {
        return x * v.x + y * v.y
    }

    fun angle(v: Float2): Float {
        return Math.toDegrees(Math.acos(normalize().dot(v.normalize()).toDouble())).toFloat()
    }

    fun rotate(angle: Float): Float2 {
        val sinA = Math.sin(Math.toRadians(angle.toDouble())).toFloat()
        val cosA = Math.cos(Math.toRadians(angle.toDouble())).toFloat()
        return Float2(cosA * x - sinA * y, sinA * x + cosA * y)
    }

    override fun toString(): String {
        return "($x, $y)"
    }

    companion object {
        val posX: Float2
            get() = Float2(1.0f, 0.0f)
        val negX: Float2
            get() = Float2(-1.0f, 0.0f)
        val posY: Float2
            get() = Float2(0.0f, 1.0f)
        val negY: Float2
            get() = Float2(0.0f, -1.0f)
    }
}