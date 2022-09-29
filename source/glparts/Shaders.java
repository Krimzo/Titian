package glparts;

import callback.EmptyCallback;
import math.*;
import utility.File;
import window.*;

import java.io.*;
import java.util.*;

import static org.lwjgl.opengl.GL33.*;

public class Shaders extends GLObject implements Serializable {
    private String vSource;
    private String fSource;
    private transient int program;

    private transient final Map<String, Integer> uniforms = new HashMap<>();

    public Shaders(GLContext context, String vSource, String fSource) {
        super(null, null, context);

        this.vSource = vSource;
        this.fSource = fSource;

        int vShader = compileShader(GL_VERTEX_SHADER, vSource);
        int fShader = compileShader(GL_FRAGMENT_SHADER, fSource);
        program = createProgram(vShader, fShader);

        glDeleteShader(vShader);
        glDeleteShader(fShader);
    }

    public Shaders(GLContext context, String filepath) {
        this(context, File.parseShader(filepath, GL_VERTEX_SHADER), File.parseShader(filepath, GL_FRAGMENT_SHADER));
    }

    @Override
    public void dispose() {
        if (program != 0) {
            glDeleteProgram(program);
            program = 0;
        }
    }

    @Serial
    private void readObject(ObjectInputStream stream) throws IOException, ClassNotFoundException {
        stream.defaultReadObject();
        Shaders shaders = new Shaders(null, (String) stream.readObject(), (String) stream.readObject());
        vSource = shaders.vSource;
        fSource = shaders.fSource;
        program = shaders.program;
    }

    private int getUniformID(String name) {
        if (uniforms.containsKey(name)) {
            return uniforms.get(name);
        }

        int uniformID = glGetUniformLocation(program, name);
        if (uniformID == -1) {
            throw new Error("Uniform \"" + name + "\" does not exist!");
        }

        uniforms.put(name, uniformID);
        return uniformID;
    }

    public void setUniform(String name, int data) {
        glUseProgram(program);
        glUniform1i(getUniformID(name), data);
        glUseProgram(0);
    }

    public void setUniform(String name, float data) {
        glUseProgram(program);
        glUniform1f(getUniformID(name), data);
        glUseProgram(0);
    }

    public void setUniform(String name, Float2 data) {
        glUseProgram(program);
        glUniform2f(getUniformID(name), data.x, data.y);
        glUseProgram(0);
    }

    public void setUniform(String name, Float3 data) {
        glUseProgram(program);
        glUniform3f(getUniformID(name), data.x, data.y, data.z);
        glUseProgram(0);
    }

    public void setUniform(String name, Float4 data) {
        glUseProgram(program);
        glUniform4f(getUniformID(name), data.x, data.y, data.z, data.w);
        glUseProgram(0);
    }

    public void setUniform(String name, Mat3 data) {
        glUseProgram(program);
        glUniformMatrix3fv(getUniformID(name), true, data.data);
        glUseProgram(0);
    }

    public void setUniform(String name, Mat4 data) {
        glUseProgram(program);
        glUniformMatrix4fv(getUniformID(name), true, data.data);
        glUseProgram(0);
    }

    public void use(EmptyCallback callback) {
        glUseProgram(program);
        callback.method();
        glUseProgram(0);
    }

    private static int compileShader(int type, String source) {
        int shader = glCreateShader(type);
        if (shader == 0) {
            throw new Error("Could not create a shader!");
        }

        glShaderSource(shader, source);
        glCompileShader(shader);

        int[] status = new int[1];
        glGetShaderiv(shader, GL_COMPILE_STATUS, status);
        if (status[0] == 0) {
            throw new Error("\n" + glGetShaderInfoLog(shader));
        }

        return shader;
    }

    private static int createProgram(int vShader, int fShader) {
        int program = glCreateProgram();
        if (program == 0) {
            throw new Error("Could not create a shader program!");
        }

        glAttachShader(program, vShader);
        glAttachShader(program, fShader);

        glLinkProgram(program);

        int[] res = new int[1];
        glGetProgramiv(program, GL_LINK_STATUS, res);
        if (res[0] == 0) {
            throw new Error("\n" + glGetProgramInfoLog(program));
        }

        return program;
    }


}
