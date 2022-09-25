package math;

import java.io.Serializable;

public class Ray implements Serializable {
    public Float3 origin;
    public Float3 direction;

    public Ray(Float3 origin, Float3 direction) {
        this.origin = origin;
        this.direction = direction;
    }
    public Ray(Ray r) {
        origin = new Float3(r.origin);
        direction = new Float3(r.direction);
    }

    // Intersection
    boolean intersect(Plane plane, Float3 outInter) {
        final float dnDot = direction.dot(plane.normal);
        if (dnDot != 0.0f) {
            if (outInter != null) {
                outInter = origin.sub(direction.mul(origin.sub(plane.point).dot(plane.normal) / dnDot));
            }
            return true;
        }
        return false;
    }
    boolean intersect(Triangle triangle, Float3 outInter) {
        // final epsilon
        final float bias = 1e-7f;

        // Calculating triangle edges
        final Float3 edge1 = triangle.b.world.sub(triangle.a.world);
        final Float3 edge2 = triangle.c.world.sub(triangle.a.world);

        // More calculations
        final Float3 h = direction.cross(edge2);
        final float a = edge1.dot(h);

        // Parallel check
        if (a > -bias && a < bias) {
            return false;
        }

        // More calculations
        final Float3 s = origin.sub(triangle.a.world);
        final float f = 1.0f / a;
        final float u = f * s.dot(h);

        // More checks
        if (u < 0.0f || u > 1.0f) {
            return false;
        }

        // More calculations
        final Float3 q = s.cross(edge1);
        final float v = f * direction.dot(q);

        // More checks
        if (v < 0.0f || u + v > 1.0f) {
            return false;
        }

        // More calculations that I don't understand
        final float t = f * edge2.dot(q);

        // Ray intersection
        if (t > bias) {
            if (outInter != null) {
                outInter = origin.add(direction.mul(t));
            }
            return true;
        }

        // Line intersection but no ray intersection
        return false;
    }
    boolean intersect(Sphere sphere, Float3 outInter) {
        // Ray sphere center ray
        final Float3 centerRay = sphere.center.sub(origin);

        // Center ray and main ray direction dot product
        final float cdDot = centerRay.dot(direction);
        if (cdDot < 0.0f) {
            return false;
        }

        // Calculations I don't understand
        final float ccDot = centerRay.dot(centerRay) - cdDot * cdDot;
        final float rr = sphere.radius * sphere.radius;
        if (ccDot > rr) {
            return false;
        }

        // Intersect distance calculation
        final float thc = (float)Math.sqrt(rr - ccDot);
        final float dis0 = cdDot - thc;
        final float dis1 = cdDot + thc;

        // Origin in sphere test
        if (dis0 < 0.0f) {
            if (outInter != null) {
                outInter = origin.add(direction.mul(dis1));
            }
        }
        else {
            if (outInter != null) {
                outInter = origin.add(direction.mul(dis0));
            }
        }
        return true;
    }
}
