package glparts;

import glparts.abs.GLObject;
import math.Float3;
import math.Vertex;
import named.NameHolder;
import utility.Memory;
import utility.Pair;
import utility.helper.FileHelper;
import window.GLContext;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.Serial;
import java.io.Serializable;
import java.util.Arrays;

import static org.lwjgl.opengl.GL33.*;

public class Mesh extends GLObject implements Serializable {
    private final Vertex[] vertices;
    public final float maxRadius;

    private transient int vao;
    private transient int vbo;

    public Mesh(NameHolder holder, String name, GLContext context, Vertex[] vertices) {
        super(holder, name, context);

        this.vertices = Arrays.copyOf(vertices, vertices.length);

        float maxDistance = 0;
        for (Vertex vertex : vertices) {
            maxDistance = Math.max(maxDistance, vertex.world.length());
        }
        maxRadius = maxDistance;

        Pair<Integer> result = generateMesh(vertices);
        vao = result.first;
        vbo = result.second;
    }

    public Mesh(NameHolder holder, String name, GLContext context, String filePath) throws Exception {
        this(holder, name, context, FileHelper.parseMeshFile(filePath));
    }

    @Override
    public void deallocate() {
        glDeleteVertexArrays(vao);
        glDeleteBuffers(vbo);
        vao = 0;
        vbo = 0;
    }

    @Override
    public boolean isAllocated() {
        return vao != 0 || vbo != 0;
    }

    @Serial
    private void readObject(ObjectInputStream stream) throws IOException, ClassNotFoundException {
        stream.defaultReadObject();
        Pair<Integer> result = generateMesh(vertices);
        vao = result.first;
        vbo = result.second;
    }

    public int getVertexCount() {
        return vertices.length;
    }

    public void renderCustom(int type, Shaders shaders) {
        shaders.use(() -> {
            glBindVertexArray(vao);
            glDrawArrays(type, 0, vertices.length);
            glBindVertexArray(0);
        });
    }

    public void renderPoints(Shaders shaders) {
        renderCustom(GL_POINTS, shaders);
    }

    public void renderLines(Shaders shaders) {
        renderCustom(GL_LINES, shaders);
    }

    public void renderTriangles(Shaders shaders) {
        renderCustom(GL_TRIANGLES, shaders);
    }

    public static Pair<Integer> generateMesh(Vertex[] vertices) {
        int vao = glGenVertexArrays();
        int vbo = glGenBuffers();

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glBufferData(GL_ARRAY_BUFFER, Memory.convertVertices(vertices), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(0, 3, GL_FLOAT, false, 32, 0);
        glVertexAttribPointer(1, 2, GL_FLOAT, false, 32, 12);
        glVertexAttribPointer(2, 3, GL_FLOAT, false, 32, 20);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        return new Pair<>(vao, vbo);
    }

    public static Mesh generateScreenMesh() {
        return new Mesh(null, "Screen Mesh", null, new Vertex[] {
            new Vertex(new Float3(-1.0f, -1.0f, 0.5f)), new Vertex(new Float3(-1.0f, 1.0f, 0.5f)), new Vertex(new Float3(1.0f, 1.0f, 0.5f)),
            new Vertex(new Float3(1.0f, 1.0f, 0.5f)), new Vertex(new Float3(1.0f, -1.0f, 0.5f)), new Vertex(new Float3(-1.0f, -1.0f, 0.5f))
        });
    }

    public static Mesh generateFrustumMesh() {
        Vertex[] frustumPoints = {
            new Vertex(new Float3( 1,  1, 0)),
            new Vertex(new Float3(-1,  1, 0)),
            new Vertex(new Float3( 1, -1, 0)),
            new Vertex(new Float3(-1, -1, 0)),

            new Vertex(new Float3( 1,  1, 1)),
            new Vertex(new Float3(-1,  1, 1)),
            new Vertex(new Float3( 1, -1, 1)),
            new Vertex(new Float3(-1, -1, 1)),
        };

        return new Mesh(null, "Frustum Mesh", null, new Vertex[] {
            frustumPoints[0], frustumPoints[1],
            frustumPoints[0], frustumPoints[2],
            frustumPoints[3], frustumPoints[1],
            frustumPoints[3], frustumPoints[2],

            frustumPoints[4], frustumPoints[5],
            frustumPoints[4], frustumPoints[6],
            frustumPoints[7], frustumPoints[5],
            frustumPoints[7], frustumPoints[6],

            frustumPoints[0], frustumPoints[4],
            frustumPoints[1], frustumPoints[5],
            frustumPoints[2], frustumPoints[6],
            frustumPoints[3], frustumPoints[7],
        });
    }

    public static Mesh generateGridMesh(int size) {
        final int sideLen = size * 2 + 1;
        Vertex[] vertices = new Vertex[sideLen * 4];

        for (int i = 0; i < sideLen; i++) {
            final int pos = -size + i;

            vertices[i * 4 + 0] = new Vertex(new Float3(pos, 0, -size));
            vertices[i * 4 + 1] = new Vertex(new Float3(pos, 0, +size));

            vertices[i * 4 + 2] = new Vertex(new Float3(-size, 0, pos));
            vertices[i * 4 + 3] = new Vertex(new Float3(+size, 0, pos));
        }

        return new Mesh(null, "Grid Mesh", null, vertices);
    }
}
