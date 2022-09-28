package glparts;

import interfaces.Renderable;
import math.*;
import utility.File;
import utility.Memory;
import window.*;

import java.io.*;
import java.util.Arrays;

import static org.lwjgl.opengl.GL33.*;

public class Mesh extends GLObject implements Renderable, Serializable {
    private Vertex[] vertices;
    private transient int vao;
    private transient int vbo;

    public Mesh(GLContext context, Vertex[] vertices) {
        super(context);

        this.vertices = Arrays.copyOf(vertices, vertices.length);

        vao = glGenVertexArrays();
        vbo = glGenBuffers();

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
    }

    public Mesh(GLContext context, String filePath) throws Exception {
        this(context, File.parseMeshFile(filePath));
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
    private void writeObject(ObjectOutputStream stream) throws IOException {
        stream.defaultWriteObject();
        stream.writeObject(vertices);
    }

    @Serial
    private void readObject(ObjectInputStream stream) throws IOException, ClassNotFoundException {
        stream.defaultReadObject();
        Mesh mesh = new Mesh(null, (Vertex[]) stream.readObject());
        vertices = mesh.vertices;
        vao = mesh.vao;
        vbo = mesh.vbo;
    }

    @Override
    public void render(Shaders shaders) {
        shaders.use(() -> {
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, vertices.length);
            glBindVertexArray(0);
        });
    }
}
