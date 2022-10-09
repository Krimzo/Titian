package math;

import java.io.Serializable;

public class Ray implements Serializable {
    public Float3 origin;
    public Float3 direction;

    public Ray(Float3 origin, Float3 direction) {
        this.origin = origin;
        this.direction = direction;
    }

    public Ray(Ray ray) {
        origin = new Float3(ray.origin);
        direction = new Float3(ray.direction);
    }

    public boolean intersect(Plane plane, Float3 outIntersection) {
        final float denom = plane.normal.normalize().dot(direction.normalize());

        if (Math.abs(denom) > 0.0001f) {
		    final float t = plane.point.subtract(origin).dot(plane.normal) / denom;

            if (t >= 0.0f) {
                if (outIntersection != null) {
                    outIntersection.x = origin.x + direction.x * t;
                    outIntersection.y = origin.y + direction.y * t;
                    outIntersection.z = origin.z + direction.z * t;
                }
                return true;
            }
        }

        return false;
    }

    public boolean intersect(Triangle triangle, Float3 outIntersection) {
        final Float3 edge1 = triangle.b.world.subtract(triangle.a.world);
        final Float3 edge2 = triangle.c.world.subtract(triangle.a.world);
    
        final Float3 h = direction.cross(edge2);
        final Float3 s = origin.subtract(triangle.a.world);
        final float f = 1 / edge1.dot(h);
        final float u = s.dot(h) * f;
        if (u < 0 || u > 1) {
            return false;
        }

        final Float3 q = s.cross(edge1);
        final float v = direction.dot(q) * f;
        if (v < 0 || (u + v) > 1) {
            return false;
        }

        final float t = edge2.dot(q) * f;
        if (t > 0) {
            if (outIntersection != null) {
			    outIntersection.x = origin.x + direction.x * t;
                outIntersection.y = origin.y + direction.y * t;
                outIntersection.z = origin.z + direction.z * t;
            }
            return true;
        }
        return false;
    }

    public boolean intersectSphere(Sphere sphere, Float4 outData) {
        final Float3 centerRay = sphere.center.subtract(origin);
        final float cdDot = centerRay.dot(direction);
        if (cdDot < 0) {
            return false;
        }

        final float ccDot = centerRay.dot(centerRay) - cdDot * cdDot;
        final float rr = sphere.radius * sphere.radius;
        if (ccDot > rr) {
            return false;
        }

        final float thc = (float) Math.sqrt(rr - ccDot);
        final float dis0 = cdDot - thc;
        final float dis1 = cdDot + thc;

        outData.w = ((dis0 < 0) ? dis1 : dis0);
        outData.x = origin.x + direction.x * outData.w;
        outData.y = origin.y + direction.y * outData.w;
        outData.z = origin.z + direction.z * outData.w;

        return true;
    }

    public boolean intersectSphere(Sphere sphere) {
        final float rayLength = sphere.center.subtract(origin).dot(direction);
        final Float3 rayPoint = direction.multiply(rayLength).add(origin);
        final float sphereRayDistance = sphere.center.subtract(rayPoint).length();
        return !(sphereRayDistance > sphere.radius);
    }
}
