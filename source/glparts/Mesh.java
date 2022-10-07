package glparts;

import glparts.abs.GLObject;
import math.*;
import named.NameHolder;
import utility.Files;
import utility.Memory;
import utility.Pair;
import window.*;

import java.io.*;
import java.util.Arrays;

import static org.lwjgl.opengl.GL33.*;

public class Mesh extends GLObject implements Serializable {
    private final Vertex[] vertices;
    private transient int vao;
    private transient int vbo;

    public Mesh(NameHolder holder, String name, GLContext context, Vertex[] vertices) {
        super(holder, name, context);

        this.vertices = Arrays.copyOf(vertices, vertices.length);

        Pair<Integer> result = generateMesh(vertices);
        vao = result.first;
        vbo = result.second;
    }

    public Mesh(NameHolder holder, String name, GLContext context, String filePath) throws Exception {
        this(holder, name, context, Files.parseMeshFile(filePath));
    }

    @Override
    public void dispose() {
        if (vao != 0) {
            glDeleteVertexArrays(vao);
            vao = 0;
        }
        if (vbo != 0) {
            glDeleteBuffers(vbo);
            vbo = 0;
        }
    }

    @Serial
    private void readObject(ObjectInputStream stream) throws IOException, ClassNotFoundException {
        stream.defaultReadObject();
        Pair<Integer> result = generateMesh(vertices);
        vao = result.first;
        vbo = result.second;
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
}
