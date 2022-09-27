package glparts;

import editor.Editor;
import math.*;
import renderer.*;
import utility.File;
import window.*;

import java.io.*;
import java.util.Arrays;

import static org.lwjgl.opengl.GL33.*;

public class Mesh extends GLObject implements Bindable, Renderable, Serializable {
    protected Vertex[] vertices = null;
    protected transient int vao = 0;
    protected transient int vbo = 0;

    public Mesh(GLContext context, Vertex[] vertices) {
        super(context);
        loadData(vertices);
    }

    public Mesh(GLContext context, String filePath) throws Exception {
        this(context, File.parseMeshFile(filePath));
    }

    public void loadData(Vertex[] vertices) {
        if (vertices != this.vertices) {
            this.vertices = Arrays.copyOf(vertices, vertices.length);
        }

        dispose();

        vao = glGenVertexArrays();
        vbo = glGenBuffers();

        bind();

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, toRawData(vertices), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, false, 32, 0);
        glVertexAttribPointer(1, 2, GL_FLOAT, false, 32, 12);
        glVertexAttribPointer(2, 3, GL_FLOAT, false, 32, 20);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        unbind();

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    @Override
    public void dispose() {
        unbind();

        if(vao != 0) {
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

        if (Editor.DEBUG) {
            System.out.println("Mesh saved!");
        }
    }

    @Serial
    private void readObject(ObjectInputStream stream) throws IOException, ClassNotFoundException {
        stream.defaultReadObject();
        vertices = (Vertex[]) stream.readObject();
        loadData(vertices);

        if (Editor.DEBUG) {
            System.out.println("Mesh loaded!");
        }
    }

    @Override
    public void bind() {
        glBindVertexArray(vao);
    }

    @Override
    public void unbind() {
        glBindVertexArray(0);
    }

    @Override
    public void onRender(Shaders shaders) {
        bind();
        glDrawArrays(GL_TRIANGLES, 0, vertices.length);
        unbind();
    }

    private static float[] toRawData(Vertex[] vertices) {
        float[] buffer = new float[vertices.length * 8];
        for(int i = 0; i < vertices.length; i++) {
            buffer[i * 8] = vertices[i].world.x;
            buffer[i * 8 + 1] = vertices[i].world.y;
            buffer[i * 8 + 2] = vertices[i].world.z;
            buffer[i * 8 + 3] = vertices[i].texture.x;
            buffer[i * 8 + 4] = vertices[i].texture.y;
            buffer[i * 8 + 5] = vertices[i].normal.x;
            buffer[i * 8 + 6] = vertices[i].normal.y;
            buffer[i * 8 + 7] = vertices[i].normal.z;
        }
        return buffer;
    }
}
