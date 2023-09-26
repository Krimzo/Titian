package sdk.math.ray

import sdk.math.cross
import sdk.math.dot
import sdk.math.normalize
import sdk.math.triangle.Triangle
import sdk.math.vector.Float3
import sdk.math.vector.Float4
import java.io.Serializable
import kotlin.math.abs
import kotlin.math.sqrt

class Ray : Serializable {
    var origin: Float3
    var direction: Float3 = Float3()
        set(value) {
            field = normalize(value)
        }

    // Constructor
    constructor() : this(Float3(), Float3.POSITIVE_Z)

    constructor(origin: Float3, direction: Float3) {
        this.origin = origin
        this.direction = direction
    }

    // Intersect
    fun intersectPlane(plane: Plane, outIntersection: Float3?): Boolean {
        val denom = dot(plane.normal, direction)
        if (abs(denom) <= 0.0001f) {
            return false
        }

        val t = dot(plane.origin - origin, plane.normal) / denom
        if (t < 0f) {
            return false
        }

        outIntersection?.let {
            val inter = origin + direction * t
            it.x = inter.x
            it.y = inter.y
            it.z = inter.z
        }
        return true
    }

    fun intersectTriangle(triangle: Triangle, outIntersection: Float3?): Boolean {
        val edge1 = triangle.b.world - triangle.a.world
        val edge2 = triangle.c.world - triangle.a.world

        val h = cross(direction, edge2)
        val s = origin - triangle.a.world
        val f = 1f / dot(edge1, h)
        val u = dot(s, h) * f
        if (u < 0f || u > 1f) {
            return false
        }

        val q = cross(s, edge1)
        val v = dot(direction, q) * f
        if (v < 0f || (u + v) > 1f) {
            return false
        }

        val t = dot(edge2, q) * f
        if (t <= 0f) {
            return false
        }

        outIntersection?.let {
            val inter = origin + direction * t
            it.x = inter.x
            it.y = inter.y
            it.z = inter.z
        }
        return true
    }

    fun canIntersectSphere(sphere: Sphere): Boolean {
        val rayDistance = dot(sphere.origin - origin, direction)
        val rayPoint = origin + direction * rayDistance
        val sphereRayDistance = (sphere.origin - rayPoint).length
        return !(sphereRayDistance > sphere.radius)
    }

    fun intersectSphere(sphere: Sphere, outIntersection: Float4?): Boolean {
        val centerRay = sphere.origin - origin
        val cdDot = dot(centerRay, direction)
        if (cdDot < 0f) {
            return false
        }

        val ccDot = dot(centerRay, centerRay) - cdDot * cdDot
        val rr = sphere.radius * sphere.radius
        if (ccDot > rr) {
            return false
        }

        val thc = sqrt(rr - ccDot)
        val dis0 = cdDot - thc
        val dis1 = cdDot + thc

        outIntersection?.let {
            it.w = if (dis0 < 0f) dis1 else dis0
            it.x = origin.x + direction.x * it.w
            it.y = origin.y + direction.y * it.w
            it.z = origin.z + direction.z * it.w
        }
        return true
    }

    // Override
    override fun toString(): String {
        return "Ray($origin, $direction)"
    }
}
