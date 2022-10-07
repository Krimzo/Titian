package math;

import java.io.Serializable;

public class Vertex implements Serializable {
    public Float3 world;
    public Float2 texture;
    public Float3 normal;

    public Vertex() {
        world = new Float3();
        texture = new Float2();
        normal = new Float3();
    }

    public Vertex(Float3 world) {
        this.world = world;
        texture = new Float2();
        normal = new Float3();
    }

    public Vertex(Float3 world, Float2 texture) {
        this.world = world;
        this.texture = texture;
        normal = new Float3();
    }

    public Vertex(Float3 world, Float2 texture, Float3 normal) {
        this.world = world;
        this.texture = texture;
        this.normal = normal;
    }

    public Vertex(Vertex vertex) {
        world = new Float3(vertex.world);
        texture = new Float2(vertex.texture);
        normal = new Float3(vertex.normal);
    }

    public String toString() {
        return "(" + world + ", " + texture + ", " + normal + ")";
    }
}
