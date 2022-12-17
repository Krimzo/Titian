package math

import java.io.Serializable
import kotlin.math.abs
import kotlin.math.sqrt

class Ray : Serializable {
    var origin: Float3 = Float3()
    var direction: Float3 = Float3()

    constructor()

    constructor(origin: Float3, direction: Float3) {
        this.origin = origin
        this.direction = direction
    }

    constructor(ray: Ray) {
        origin = Float3(ray.origin)
        direction = Float3(ray.direction)
    }

    fun intersect(plane: Plane, outIntersection: Float3): Boolean {
        val denom = normalize(plane.normal) * normalize(direction)
        if (abs(denom) > 0.0001f) {
            val t = (plane.point - origin) * plane.normal / denom
            if (t >= 0f) {
                val res = origin + direction * t
                outIntersection.x = res.x
                outIntersection.y = res.y
                outIntersection.z = res.z
                return true
            }
        }
        return false
    }

    fun intersect(triangle: Triangle, outIntersection: Float3): Boolean {
        val edge1 = triangle.b.world - triangle.a.world
        val edge2 = triangle.c.world - triangle.a.world

        val h = direction x edge2
        val s = origin - triangle.a.world
        val f = 1f / (edge1 * h)
        val u = s * h * f
        if (u < 0f || u > 1f) {
            return false
        }

        val q = s x edge1
        val v = direction * q * f
        if (v < 0f || (u + v) > 1f) {
            return false
        }

        val t = edge2 * q * f
        if (t > 0f) {
            val res = origin + direction * t
            outIntersection.x = res.x
            outIntersection.y = res.y
            outIntersection.z = res.z
            return true
        }
        return false
    }

    fun intersectSphere(sphere: Sphere, outData: Float4? = null): Boolean {
        val centerRay = sphere.center - origin
        val cdDot = centerRay * direction
        if (cdDot < 0f) {
            return false
        }

        val ccDot = centerRay * centerRay - cdDot * cdDot
        val rr = sphere.radius * sphere.radius
        if (ccDot > rr) {
            return false
        }

        val thc = sqrt(rr - ccDot)
        val dis0 = cdDot - thc
        val dis1 = cdDot + thc
        outData?.let {
            it.w = if (dis0 < 0f) dis1 else dis0
            it.x = origin.x + direction.x * it.w
            it.y = origin.y + direction.y * it.w
            it.z = origin.z + direction.z * it.w
        }
        return true
    }

    fun intersectSphere(sphere: Sphere): Boolean {
        val rayLength = (sphere.center - origin) * direction
        val rayPoint = direction * rayLength + origin
        val sphereRayDistance = (sphere.center - rayPoint).length
        return sphereRayDistance <= sphere.radius
    }
}