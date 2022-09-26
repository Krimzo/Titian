package glparts;

import math.*;
import utility.File;
import window.*;

import java.util.*;

import static org.lwjgl.opengl.GL33.*;

public class Shaders extends GLObject implements Validated, Bindable {
    protected int program;

    public Shaders(GLContext context, String vSource, String fSource) {
        super(context);

        final int vShader = newShader(GL_VERTEX_SHADER, vSource);
        final int fShader = newShader(GL_FRAGMENT_SHADER, fSource);
        program = newProgram(vShader, fShader);

        glDeleteShader(vShader);
        glDeleteShader(fShader);
    }

    public Shaders(GLContext context, String filePath) throws Exception {
        this(context, File.parseShader(filePath, GL_VERTEX_SHADER), File.parseShader(filePath, GL_FRAGMENT_SHADER));
    }

    @Override
    public void destroy() {
        if (isValid()) {
            unbind();
            glDeleteProgram(program);
            program = 0;
        }
    }

    @Override
    public boolean isValid() {
        return program != 0;
    }

    @Override
    public void bind() {
        glUseProgram(program);
    }

    @Override
    public void unbind() {
        glUseProgram(0);
    }

    private final Map<String, Integer> uniforms = new HashMap<>();
    private int getUniformID(String name) {
        if (uniforms.containsKey(name)) {
            return uniforms.get(name);
        }
        final int uniformID = glGetUniformLocation(program, name);
        if (uniformID == -1) {
            throw new Error("Uniform \"" + name + "\" does not exist!");
        }
        uniforms.put(name, uniformID);
        return uniformID;
    }

    public void setUniform(String name, int data) {
        bind();
        glUniform1i(getUniformID(name), data);
    }
    public void setUniform(String name, float data) {
        bind();
        glUniform1f(getUniformID(name), data);
    }
    public void setUniform(String name, Float2 data) {
        bind();
        glUniform2f(getUniformID(name), data.x, data.y);
    }
    public void setUniform(String name, Float3 data) {
        bind();
        glUniform3f(getUniformID(name), data.x, data.y, data.z);
    }
    public void setUniform(String name, Float4 data) {
        bind();
        glUniform4f(getUniformID(name), data.x, data.y, data.z, data.w);
    }
    public void setUniform(String name, Mat3 data) {
        bind();
        glUniformMatrix3fv(getUniformID(name), true, data.data);
    }
    public void setUniform(String name, Mat4 data) {
        bind();
        glUniformMatrix4fv(getUniformID(name), true, data.data);
    }

    private static int newShader(int type, String source) {
        final int shader = glCreateShader(type);
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

    private static int newProgram(int vShader, int fShader) {
        final int program = glCreateProgram();
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
