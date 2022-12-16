package math

import java.awt.Color
import java.io.Serializable

class Float3 : Serializable {
    var x = 0f
    var y = 0f
    var z = 0f

    constructor() {
        set(0f)
    }

    constructor(a: Float) {
        set(a)
    }

    constructor(data: FloatArray?) {
        set(data)
    }

    constructor(x: Float, y: Float, z: Float) {
        set(x, y, z)
    }

    constructor(v: Float2?, z: Float) {
        set(v, z)
    }

    constructor(x: Float, v: Float2) {
        set(x, v)
    }

    constructor(v: Float3?) {
        set(v)
    }

    constructor(v: Int3) {
        set(v)
    }

    constructor(color: Color) {
        set(color)
    }

    // Setters
    operator fun set(x: Float, y: Float, z: Float) {
        this.x = x
        this.y = y
        this.z = z
    }

    fun set(a: Float) {
        set(a, a, a)
    }

    fun set(data: FloatArray?) {
        set(data!![0], data[1], data[2])
    }

    operator fun set(v: Float2?, z: Float) {
        set(v!!.x, v.y, z)
    }

    operator fun set(x: Float, v: Float2) {
        set(x, v.x, v.y)
    }

    fun set(v: Float3?) {
        set(v!!.x, v.y, v.z)
    }

    fun set(v: Int3) {
        set(v.x.toFloat(), v.y.toFloat(), v.z.toFloat())
    }

    fun set(color: Color) {
        val conv = 1.0f / 255.0f
        set(color.red * conv, color.green * conv, color.blue * conv)
    }

    // Getters
    fun xy(): Float2 {
        return Float2(x, y)
    }

    fun array(): FloatArray {
        return floatArrayOf(x, y, z)
    }

    fun color(): Color {
        return Color(Math.min(Math.max(x * 255, 0f), 255f).toInt(), Math.min(Math.max(y * 255, 0f), 255f).toInt(), Math.min(Math.max(z * 255, 0f), 255f).toInt())
    }

    // Math
    fun add(v: Float3?): Float3 {
        return Float3(x + v!!.x, y + v.y, z + v.z)
    }

    fun subtract(v: Float3?): Float3 {
        return Float3(x - v!!.x, y - v.y, z - v.z)
    }

    fun multiply(a: Float): Float3 {
        return Float3(x * a, y * a, z * a)
    }

    fun multiply(v: Float3): Float3 {
        return Float3(x * v.x, y * v.y, z * v.z)
    }

    fun divide(a: Float): Float3 {
        return multiply(1.0f / a)
    }

    fun divide(v: Float3): Float3 {
        return Float3(x / v.x, y / v.y, z / v.z)
    }

    fun equals(v: Float3): Boolean {
        return x == v.x && y == v.y && z == v.z
    }

    fun negate(): Float3 {
        return multiply(-1.0f)
    }

    fun absolute(): Float3 {
        return Float3(Math.abs(x), Math.abs(y), Math.abs(z))
    }

    fun length(): Float {
        return Math.sqrt(dot(this).toDouble()).toFloat()
    }

    fun normalize(): Float3 {
        return divide(length())
    }

    fun dot(v: Float3?): Float {
        return x * v!!.x + y * v.y + z * v.z
    }

    fun cross(v: Float3?): Float3 {
        return Float3(y * v!!.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x)
    }

    fun angle(v: Float3?): Float {
        return Math.toDegrees(Math.acos(normalize().dot(v!!.normalize()).toDouble())).toFloat()
    }

    fun rotate(angle: Float, axis: Float3?): Float3 {
        val angleSin = Math.sin(Math.toRadians((angle * 0.5f).toDouble())).toFloat()
        val angleCos = Math.cos(Math.toRadians((angle * 0.5f).toDouble())).toFloat()
        val qx = axis!!.x * angleSin
        val qy = axis.y * angleSin
        val qz = axis.z * angleSin
        val x2 = qx * qx
        val y2 = qy * qy
        val z2 = qz * qz
        val w2 = angleCos * angleCos
        val xy = qx * qy
        val xz = qx * qz
        val yz = qy * qz
        val xw = qx * angleCos
        val yw = qy * angleCos
        val zw = qz * angleCos
        val temp = Float3(0f)
        temp.x = (w2 + x2 - z2 - y2) * x + (-zw + xy - zw + xy) * y + (yw + xz + xz + yw) * z
        temp.y = (xy + zw + zw + xy) * x + (y2 - z2 + w2 - x2) * y + (yz + yz - xw - xw) * z
        temp.z = (xz - yw + xz - yw) * x + (yz + yz + xw + xw) * y + (z2 - y2 - x2 + w2) * z
        return temp
    }

    fun reflect(v: Float3): Float3 {
        val normal = v.normalize()
        return this.subtract(normal.multiply(2 * dot(normal)))
    }

    override fun toString(): String {
        return "($x, $y, $z)"
    }

    companion object {
        val posX: Float3
            get() = Float3(1.0f, 0.0f, 0.0f)
        val negX: Float3
            get() = Float3(-1.0f, 0.0f, 0.0f)
        val posY: Float3
            get() = Float3(0.0f, 1.0f, 0.0f)
        val negY: Float3
            get() = Float3(0.0f, -1.0f, 0.0f)
        val posZ: Float3
            get() = Float3(0.0f, 0.0f, 1.0f)
        val negZ: Float3
            get() = Float3(0.0f, 0.0f, -1.0f)
    }
}