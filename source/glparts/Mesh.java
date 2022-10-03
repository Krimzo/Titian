package glparts;

import glparts.abs.GLObject;
import renderer.abs.Renderable;
import math.*;
import named.NameHolder;
import utility.Files;
import utility.Memory;
import utility.Pair;
import window.*;

import java.io.*;
import java.util.Arrays;

import static org.lwjgl.opengl.GL33.*;

public class Mesh extends GLObject implements Renderable, Serializable {
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

    @Override
    public void render(Shaders shaders) {
        shaders.use(() -> {
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, vertices.length);
            glBindVertexArray(0);
        });
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
        return new Mesh(null, null, null, new Vertex[] {
            new Vertex(new Float3(-1.0f, -1.0f, 0.5f)), new Vertex(new Float3(-1.0f, 1.0f, 0.5f)), new Vertex(new Float3(1.0f, 1.0f, 0.5f)),
            new Vertex(new Float3(1.0f, 1.0f, 0.5f)), new Vertex(new Float3(1.0f, -1.0f, 0.5f)), new Vertex(new Float3(-1.0f, -1.0f, 0.5f))
        });
    }
}
