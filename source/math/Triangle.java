package math;

import java.io.Serializable;

public class Triangle implements Serializable {
    public Vertex a;
    public Vertex b;
    public Vertex c;

    public Triangle(Vertex A, Vertex B, Vertex C) {
        a = A;
        b = B;
        c = C;
    }

    public Triangle(Triangle triangle) {
        a = new Vertex(triangle.a);
        b = new Vertex(triangle.b);
        c = new Vertex(triangle.c);
    }
}
