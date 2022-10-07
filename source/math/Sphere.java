package math;

import java.io.Serializable;

public class Sphere implements Serializable {
    public Float3 center;
    public float radius;

    public Sphere(Float3 center, float radius) {
        this.center = center;
        this.radius = radius;
    }

    public Sphere(Sphere sphere) {
        center = new Float3(sphere.center);
        radius = sphere.radius;
    }
}
