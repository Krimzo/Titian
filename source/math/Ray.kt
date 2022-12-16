package math

import java.io.Serializable

class Ray : Serializable {
    var origin: Float3
    var direction: Float3

    constructor(origin: Float3, direction: Float3) {
        this.origin = origin
        this.direction = direction
    }

    constructor(ray: Ray) {
        origin = Float3(ray.origin)
        direction = Float3(ray.direction)
    }

    fun intersect(plane: Plane, outIntersection: Float3): Boolean {
        val denom = plane.normal.normalize().dot(direction.normalize())
        if (Math.abs(denom) > 0.0001f) {
            val t = plane.point.subtract(origin).dot(plane.normal) / denom
            if (t >= 0.0f) {
                outIntersection.set(origin.add(direction.multiply(t)))
                return true
            }
        }
        return false
    }

    fun intersect(triangle: Triangle, outIntersection: Float3): Boolean {
        val edge1 = triangle.b.world!!.subtract(triangle.a.world)
        val edge2 = triangle.c.world!!.subtract(triangle.a.world)
        val h = direction.cross(edge2)
        val s = origin.subtract(triangle.a.world)
        val f = 1 / edge1!!.dot(h)
        val u = s!!.dot(h) * f
        if (u < 0 || u > 1) {
            return false
        }
        val q = s.cross(edge1)
        val v = direction.dot(q) * f
        if (v < 0 || u + v > 1) {
            return false
        }
        val t = edge2!!.dot(q) * f
        if (t > 0) {
            outIntersection.set(origin.add(direction.multiply(t)))
            return true
        }
        return false
    }

    fun intersectSphere(sphere: Sphere, outData: Float4): Boolean {
        val centerRay = sphere.center.subtract(origin)
        val cdDot = centerRay!!.dot(direction)
        if (cdDot < 0) {
            return false
        }
        val ccDot = centerRay.dot(centerRay) - cdDot * cdDot
        val rr = sphere.radius * sphere.radius
        if (ccDot > rr) {
            return false
        }
        val thc = Math.sqrt((rr - ccDot).toDouble()).toFloat()
        val dis0 = cdDot - thc
        val dis1 = cdDot + thc
        outData.let {
            it.w = if (dis0 < 0f) dis1 else dis0
            it.x = origin.x + direction.x * it.w
            it.y = origin.y + direction.y * it.w
            it.z = origin.z + direction.z * it.w
        }
        return true
    }

    fun intersectSphere(sphere: Sphere): Boolean {
        val rayLength = sphere.center.subtract(origin).dot(direction)
        val rayPoint = direction.multiply(rayLength).add(origin)
        val sphereRayDistance = sphere.center.subtract(rayPoint).length()
        return sphereRayDistance <= sphere.radius
    }
}