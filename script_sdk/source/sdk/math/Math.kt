package sdk.math

import sdk.math.helper.MathConstants
import sdk.math.imaginary.Complex
import sdk.math.imaginary.Quaternion
import sdk.math.matrix.Float2x2
import sdk.math.matrix.Float3x3
import sdk.math.matrix.Float4x4
import sdk.math.vector.Float2
import sdk.math.vector.Float3
import sdk.math.vector.Float4
import sdk.math.vector.Int2
import kotlin.math.*

// Deg trig
fun sinDeg(angleInDeg: Float): Float {
    return sin(angleInDeg * MathConstants.TO_RADIANS)
}

fun cosDeg(angleInDeg: Float): Float {
    return cos(angleInDeg * MathConstants.TO_RADIANS)
}

fun tanDeg(angleInDeg: Float): Float {
    return tan(angleInDeg * MathConstants.TO_RADIANS)
}

fun asinDeg(value: Float): Float {
    return asin(value) * MathConstants.TO_DEGREES
}

fun acosDeg(value: Float): Float {
    return acos(value) * MathConstants.TO_DEGREES
}

fun atanDeg(value: Float): Float {
    return atan(value) * MathConstants.TO_DEGREES
}

// Lines
fun lineX(a: Float2, b: Float2, y: Float): Float {
    return ((y - a.y) * (b.x - a.x)) / (b.y - a.y) + a.x
}

fun lineY(a: Float2, b: Float2, x: Float): Float {
    return ((b.y - a.y) * (x - a.x)) / (b.x - a.x) + a.y
}

// Wrap
fun wrap(value: Float, lower: Float, upper: Float): Float {
    var value = (value - lower) / (upper - lower)
    value = min(max(value, 0f), 1f)
    return value
}

fun unwrap(value: Float, lower: Float, upper: Float): Float {
    var value = (upper - lower) * value + lower
    value = min(max(value, lower), upper)
    return value
}

fun clamp(value: Float, lower: Float, upper: Float): Float {
    return min(max(value, lower), upper)
}

// Rotation
fun toQuaternion(euler: Float3): Quaternion {
    val cr = cosDeg(euler.x * 0.5f)
    val sr = sinDeg(euler.x * 0.5f)
    val cp = cosDeg(euler.y * 0.5f)
    val sp = sinDeg(euler.y * 0.5f)
    val cy = cosDeg(euler.z * 0.5f)
    val sy = sinDeg(euler.z * 0.5f)

    return Quaternion(
        cr * cp * cy + sr * sp * sy,
        sr * cp * cy - cr * sp * sy,
        cr * sp * cy + sr * cp * sy,
        cr * cp * sy - sr * sp * cy,
    )
}

fun toEuler(quaternion: Quaternion): Float3 {
    val     sinP = +2f * (quaternion.w * quaternion.y - quaternion.z * quaternion.x) + 0f
    val sinRCosP = +2f * (quaternion.w * quaternion.x + quaternion.y * quaternion.z) + 0f
    val cosRCosP = -2f * (quaternion.x * quaternion.x + quaternion.y * quaternion.y) + 1f
    val sinYCosP = +2f * (quaternion.w * quaternion.z + quaternion.x * quaternion.y) + 0f
    val cosYCosP = -2f * (quaternion.y * quaternion.y + quaternion.z * quaternion.z) + 1f

    val result = Float3(
        atan2(sinRCosP, cosRCosP),
        (if (abs(sinP) >= 1f) (MathConstants.PI * 0.5f).withSign(sinP) else asin(sinP)),
        atan2(sinYCosP, cosYCosP),
    )
    return result * MathConstants.TO_DEGREES
}

// Complex
fun abs(num: Complex): Complex {
    return Complex(abs(num.r), abs(num.i))
}

fun normalize(num: Complex): Complex {
    return num * (1f / num.length)
}

fun inverse(num: Complex): Complex {
    val sqrSum = (num.r * num.r + num.i * num.i)
    if (sqrSum != 0f) {
        return Complex(num.r / sqrSum, -num.i / sqrSum)
    }
    return Complex()
}

// Quaternion
fun abs(num: Quaternion): Quaternion {
    return Quaternion(abs(num.w), abs(num.x), abs(num.y), abs(num.z))
}

fun normalize(num: Quaternion): Quaternion {
    return num * (1f / num.length)
}

fun inverse(num: Quaternion): Quaternion {
    return Quaternion(num.w, -num.x, -num.y, -num.z)
}

// Int2
fun abs(vec: Int2): Int2 {
    return Int2(abs(vec.x), abs(vec.y))
}

// Float2
fun abs(vec: Float2): Float2 {
    return Float2(abs(vec.x), abs(vec.y))
}

fun normalize(vec: Float2): Float2 {
    return vec / vec.length
}

fun dot(first: Float2, second: Float2): Float {
    return first.x * second.x + first.y * second.y
}

fun angle(first: Float2, second: Float2, full: Boolean): Float {
    val result = if (full) {
        atan2(first.x * second.y - first.y * second.x, first.x * second.x + first.y * second.y)
    }
    else {
        acos(dot(normalize(first), normalize(second)))
    }
    return result * MathConstants.TO_DEGREES
}

fun rotate(vec: Float2, angle: Float): Float2 {
    val sinA = sinDeg(angle)
    val cosA = cosDeg(angle)
    return Float2(cosA * vec.x - sinA * vec.y, sinA * vec.x + cosA * vec.y)
}

fun reflect(vec: Float2, point: Float2): Float2 {
    val normalized = normalize(point)
    return vec - (normalized * (vec * normalized) * 2f)
}

// Float3
fun abs(vec: Float3): Float3 {
    return Float3(abs(vec.x), abs(vec.y), abs(vec.z))
}

fun normalize(vec: Float3): Float3 {
    return vec / vec.length
}

fun dot(first: Float3, second: Float3): Float {
    return (first.x * second.x + first.y * second.y + first.z * second.z)
}

fun angle(first: Float3, second: Float3): Float {
    return acosDeg(dot(normalize(first), normalize(second)))
}

fun rotate(vec: Float3, axis: Float3, angle: Float): Float3 {
    val quat = Quaternion(axis, angle)
    val invQuat = inverse(quat)
    return (quat * vec.quaternion * invQuat).float3
}

fun reflect(first: Float3, normal: Float3): Float3 {
    val normal = normalize(normal)
    return first - (normal * (first * normal) * 2f)
}

fun cross(first: Float3, second: Float3): Float3 {
    return Float3(
        first.y * second.z - first.z * second.y,
        first.z * second.x - first.x * second.z,
        first.x * second.y - first.y * second.x,
    )
}

// Float4
fun abs(vec: Float4): Float2 {
    return Float2(abs(vec.x), abs(vec.y))
}

fun normalize(vec: Float4): Float4 {
    return vec / vec.length
}

fun dot(first: Float4, second: Float4): Float {
    return (first.x * second.x + first.y * second.y + first.z * second.z + first.w * second.w)
}

fun angle(first: Float4, second: Float4): Float {
    return acosDeg(dot(normalize(first), normalize(second)))
}

// Float2x2
fun abs(mat: Float2x2): Float2x2 {
    val result = Float2x2()
    for (i in 0 until 4) {
        result[i] = abs(mat[i])
    }
    return result
}

fun inverse(mat: Float2x2): Float2x2 {
    val determinant = mat.determinant

    val result = Float2x2()
    result[0] = +mat[3]
    result[1] = -mat[1]
    result[2] = -mat[2]
    result[3] = +mat[0]
    return result * determinant
}

fun transpose(mat: Float2x2): Float2x2 {
    val result = Float2x2()
    for (y in 0 until 2) {
        for (x in 0 until 2) {
            result[x, y] = mat[y, x]
        }
    }
    return result
}

// Float3x3
fun abs(mat: Float3x3): Float3x3 {
    val result = Float3x3()
    for (i in 0 until 9) {
        result[i] = abs(mat[i])
    }
    return result
}

fun inverse(mat: Float3x3): Float3x3 {
    val determinant = mat.determinant

    val result = Float3x3()
    result[0, 0] = (mat[1, 1] * mat[2, 2] - mat[2, 1] * mat[1, 2]) * determinant
    result[0, 1] = (mat[0, 2] * mat[2, 1] - mat[0, 1] * mat[2, 2]) * determinant
    result[0, 2] = (mat[0, 1] * mat[1, 2] - mat[0, 2] * mat[1, 1]) * determinant
    result[1, 0] = (mat[1, 2] * mat[2, 0] - mat[1, 0] * mat[2, 2]) * determinant
    result[1, 1] = (mat[0, 0] * mat[2, 2] - mat[0, 2] * mat[2, 0]) * determinant
    result[1, 2] = (mat[1, 0] * mat[0, 2] - mat[0, 0] * mat[1, 2]) * determinant
    result[2, 0] = (mat[1, 0] * mat[2, 1] - mat[2, 0] * mat[1, 1]) * determinant
    result[2, 1] = (mat[2, 0] * mat[0, 1] - mat[0, 0] * mat[2, 1]) * determinant
    result[2, 2] = (mat[0, 0] * mat[1, 1] - mat[1, 0] * mat[0, 1]) * determinant
    return result
}

fun transpose(mat: Float3x3): Float3x3 {
    val result = Float3x3()
    for (y in 0 until 3) {
        for (x in 0 until 3) {
            result[x, y] = mat[y, x]
        }
    }
    return result
}

// Float4x4
fun abs(mat: Float4x4): Float4x4 {
    val result = Float4x4()
    for (i in 0 until 16) {
        result[i] = abs(mat[i])
    }
    return result
}

fun inverse(mat: Float4x4): Float4x4 {
    val determinant = mat.determinant

    val A2323 = mat[2, 2] * mat[3, 3] - mat[3, 2] * mat[2, 3]
    val A1323 = mat[1, 2] * mat[3, 3] - mat[3, 2] * mat[1, 3]
    val A1223 = mat[1, 2] * mat[2, 3] - mat[2, 2] * mat[1, 3]
    val A0323 = mat[0, 2] * mat[3, 3] - mat[3, 2] * mat[0, 3]
    val A0223 = mat[0, 2] * mat[2, 3] - mat[2, 2] * mat[0, 3]
    val A0123 = mat[0, 2] * mat[1, 3] - mat[1, 2] * mat[0, 3]
    val A2313 = mat[2, 1] * mat[3, 3] - mat[3, 1] * mat[2, 3]
    val A1313 = mat[1, 1] * mat[3, 3] - mat[3, 1] * mat[1, 3]
    val A1213 = mat[1, 1] * mat[2, 3] - mat[2, 1] * mat[1, 3]
    val A2312 = mat[2, 1] * mat[3, 2] - mat[3, 1] * mat[2, 2]
    val A1312 = mat[1, 1] * mat[3, 2] - mat[3, 1] * mat[1, 2]
    val A1212 = mat[1, 1] * mat[2, 2] - mat[2, 1] * mat[1, 2]
    val A0313 = mat[0, 1] * mat[3, 3] - mat[3, 1] * mat[0, 3]
    val A0213 = mat[0, 1] * mat[2, 3] - mat[2, 1] * mat[0, 3]
    val A0312 = mat[0, 1] * mat[3, 2] - mat[3, 1] * mat[0, 2]
    val A0212 = mat[0, 1] * mat[2, 2] - mat[2, 1] * mat[0, 2]
    val A0113 = mat[0, 1] * mat[1, 3] - mat[1, 1] * mat[0, 3]
    val A0112 = mat[0, 1] * mat[1, 2] - mat[1, 1] * mat[0, 2]

    val result = Float4x4()
    result[0, 0] = +(mat[1, 1] * A2323 - mat[2, 1] * A1323 + mat[3, 1] * A1223) * determinant
    result[1, 0] = -(mat[1, 0] * A2323 - mat[2, 0] * A1323 + mat[3, 0] * A1223) * determinant
    result[2, 0] = +(mat[1, 0] * A2313 - mat[2, 0] * A1313 + mat[3, 0] * A1213) * determinant
    result[3, 0] = -(mat[1, 0] * A2312 - mat[2, 0] * A1312 + mat[3, 0] * A1212) * determinant
    result[0, 1] = -(mat[0, 1] * A2323 - mat[2, 1] * A0323 + mat[3, 1] * A0223) * determinant
    result[1, 1] = +(mat[0, 0] * A2323 - mat[2, 0] * A0323 + mat[3, 0] * A0223) * determinant
    result[2, 1] = -(mat[0, 0] * A2313 - mat[2, 0] * A0313 + mat[3, 0] * A0213) * determinant
    result[3, 1] = +(mat[0, 0] * A2312 - mat[2, 0] * A0312 + mat[3, 0] * A0212) * determinant
    result[0, 2] = +(mat[0, 1] * A1323 - mat[1, 1] * A0323 + mat[3, 1] * A0123) * determinant
    result[1, 2] = -(mat[0, 0] * A1323 - mat[1, 0] * A0323 + mat[3, 0] * A0123) * determinant
    result[2, 2] = +(mat[0, 0] * A1313 - mat[1, 0] * A0313 + mat[3, 0] * A0113) * determinant
    result[3, 2] = -(mat[0, 0] * A1312 - mat[1, 0] * A0312 + mat[3, 0] * A0112) * determinant
    result[0, 3] = -(mat[0, 1] * A1223 - mat[1, 1] * A0223 + mat[2, 1] * A0123) * determinant
    result[1, 3] = +(mat[0, 0] * A1223 - mat[1, 0] * A0223 + mat[2, 0] * A0123) * determinant
    result[2, 3] = -(mat[0, 0] * A1213 - mat[1, 0] * A0213 + mat[2, 0] * A0113) * determinant
    result[3, 3] = +(mat[0, 0] * A1212 - mat[1, 0] * A0212 + mat[2, 0] * A0112) * determinant
    return result
}

fun transpose(mat: Float4x4): Float4x4 {
    val result = Float4x4()
    for (y in 0 until 4) {
        for (x in 0 until 4) {
            result[x, y] = mat[y, x]
        }
    }
    return result
}
