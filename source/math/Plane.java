package math;

import java.io.Serializable;

public class Plane implements Serializable {
    public Float3 normal;
    public Float3 point;

    public Plane(Float3 normal, Float3 point) {
        this.normal = normal;
        this.point = point;
    }

    public Plane(Plane plane) {
        normal = new Float3(plane.normal);
        point = new Float3(plane.point);
    }
}
