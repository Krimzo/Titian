package math;

import java.io.Serializable;

public class Triangle implements Serializable {
    public Vertex a, b, c;

    public Triangle(Vertex A, Vertex B, Vertex C) {
        a = A;
        b = B;
        c = C;
    }
    public Triangle(Triangle t) {
        a = new Vertex(t.a);
        b = new Vertex(t.b);
        c = new Vertex(t.c);
    }
}
