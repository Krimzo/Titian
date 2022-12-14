package glparts

import glparts.abs.GLObject
import math.*
import named.NameHolder
import org.lwjgl.opengl.GL20
import utility.helper.FileHelper
import window.GLContext
import java.io.ObjectInputStream
import java.io.Serial
import java.io.Serializable

class Shaders : GLObject, Serializable {
    private val vSource: String
    private val fSource: String

    @Transient
    private var program: Int

    @Transient
    private var uniforms: HashMap<String, Int> = HashMap()

    constructor(context: GLContext, vSource: String, fSource: String) : super(NameHolder(), "Shaders", context) {
        this.vSource = vSource
        this.fSource = fSource
        program = generateShaders(vSource, fSource)
    }

    constructor(context: GLContext, filepath: String) : super(NameHolder(), "Shaders", context) {
        this.vSource = FileHelper.parseShader(filepath, GL20.GL_VERTEX_SHADER) ?: ""
        this.fSource = FileHelper.parseShader(filepath, GL20.GL_FRAGMENT_SHADER) ?: ""
        program = generateShaders(vSource, fSource)
    }

    override fun deallocate() {
        GL20.glDeleteProgram(program)
        program = 0
    }

    override fun isAllocated(): Boolean {
        return program != 0
    }

    @Serial
    private fun readObject(stream: ObjectInputStream) {
        stream.defaultReadObject()
        program = generateShaders(vSource, fSource)
    }

    private fun getUniformID(name: String): Int {
        uniforms[name]?.let { return it }
        val uniformID = GL20.glGetUniformLocation(program, name)
        if (uniformID == -1) {
            throw Error("Uniform \"$name\" does not exist!")
        }
        uniforms[name] = uniformID
        return uniformID
    }

    fun setUniform(name: String, data: Int) {
        GL20.glUseProgram(program)
        GL20.glUniform1i(getUniformID(name), data)
        GL20.glUseProgram(0)
    }

    fun setUniform(name: String, data: Float) {
        GL20.glUseProgram(program)
        GL20.glUniform1f(getUniformID(name), data)
        GL20.glUseProgram(0)
    }

    fun setUniform(name: String, data: Float2) {
        GL20.glUseProgram(program)
        GL20.glUniform2f(getUniformID(name), data.x, data.y)
        GL20.glUseProgram(0)
    }

    fun setUniform(name: String, data: Float3) {
        GL20.glUseProgram(program)
        GL20.glUniform3f(getUniformID(name), data.x, data.y, data.z)
        GL20.glUseProgram(0)
    }

    fun setUniform(name: String, data: Float4) {
        GL20.glUseProgram(program)
        GL20.glUniform4f(getUniformID(name), data.x, data.y, data.z, data.w)
        GL20.glUseProgram(0)
    }

    fun setUniform(name: String, data: Mat3) {
        GL20.glUseProgram(program)
        GL20.glUniformMatrix3fv(getUniformID(name), true, data.data)
        GL20.glUseProgram(0)
    }

    fun setUniform(name: String, data: Mat4) {
        GL20.glUseProgram(program)
        GL20.glUniformMatrix4fv(getUniformID(name), true, data.data)
        GL20.glUseProgram(0)
    }

    fun use(callback: () -> Unit) {
        GL20.glUseProgram(program)
        callback()
        GL20.glUseProgram(0)
    }

    companion object {
        private fun compileShader(type: Int, source: String): Int {
            val shader = GL20.glCreateShader(type)
            if (shader == 0) {
                throw Error("Could not create a shader!")
            }
            GL20.glShaderSource(shader, source)
            GL20.glCompileShader(shader)
            val status = IntArray(1)
            GL20.glGetShaderiv(shader, GL20.GL_COMPILE_STATUS, status)
            if (status[0] == 0) {
                throw Error(GL20.glGetShaderInfoLog(shader))
            }
            return shader
        }

        private fun createProgram(vShader: Int, fShader: Int): Int {
            val program = GL20.glCreateProgram()
            if (program == 0) {
                throw Error("Could not create a shader program!")
            }
            GL20.glAttachShader(program, vShader)
            GL20.glAttachShader(program, fShader)
            GL20.glLinkProgram(program)
            val res = IntArray(1)
            GL20.glGetProgramiv(program, GL20.GL_LINK_STATUS, res)
            if (res[0] == 0) {
                throw Error(GL20.glGetProgramInfoLog(program))
            }
            return program
        }

        private fun generateShaders(vSource: String, fSource: String): Int {
            val vShader = compileShader(GL20.GL_VERTEX_SHADER, vSource)
            val fShader = compileShader(GL20.GL_FRAGMENT_SHADER, fSource)
            val program = createProgram(vShader, fShader)
            GL20.glDeleteShader(vShader)
            GL20.glDeleteShader(fShader)
            return program
        }
    }
}