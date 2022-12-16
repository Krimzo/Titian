package glparts

import glparts.abs.GLObject
import math.Float3
import math.Vertex
import named.NameHolder
import org.lwjgl.opengl.GL11
import org.lwjgl.opengl.GL15
import org.lwjgl.opengl.GL20
import org.lwjgl.opengl.GL30
import utility.Memory
import utility.Pair
import utility.helper.FileHelper
import window.GLContext
import java.io.IOException
import java.io.ObjectInputStream
import java.io.Serial
import java.io.Serializable

class Mesh(holder: NameHolder?, name: String?, context: GLContext?, vertices: Array<Vertex>) : GLObject(holder, name, context), Serializable {
    private val vertices: Array<Vertex>
    val maxRadius: Float

    @Transient
    private var vao: Int

    @Transient
    private var vbo: Int

    init {
        this.vertices = vertices.copyOf()
        var maxDistance = 0f
        for (vertex in vertices) {
            maxDistance = Math.max(maxDistance, vertex.world!!.length())
        }
        maxRadius = maxDistance
        val result = generateMesh(vertices)
        vao = result.first!!
        vbo = result.second!!
    }

    constructor(holder: NameHolder?, name: String?, context: GLContext?, filePath: String) : this(holder, name, context, FileHelper.parseMeshFile(filePath))

    override fun deallocate() {
        GL30.glDeleteVertexArrays(vao)
        GL15.glDeleteBuffers(vbo)
        vao = 0
        vbo = 0
    }

    override fun isAllocated(): Boolean {
        return vao != 0 || vbo != 0
    }

    @Serial
    @Throws(IOException::class, ClassNotFoundException::class)
    private fun readObject(stream: ObjectInputStream) {
        stream.defaultReadObject()
        val result = generateMesh(vertices)
        vao = result.first!!
        vbo = result.second!!
    }

    fun getVertexCount(): Int {
        return vertices.size
    }

    fun renderCustom(type: Int, shaders: Shaders?) {
        shaders!!.use {
            GL30.glBindVertexArray(vao)
            GL11.glDrawArrays(type, 0, vertices.size)
            GL30.glBindVertexArray(0)
        }
    }

    fun renderPoints(shaders: Shaders?) {
        renderCustom(GL11.GL_POINTS, shaders)
    }

    fun renderLines(shaders: Shaders?) {
        renderCustom(GL11.GL_LINES, shaders)
    }

    fun renderTriangles(shaders: Shaders?) {
        renderCustom(GL11.GL_TRIANGLES, shaders)
    }

    companion object {
        fun generateMesh(vertices: Array<Vertex>): Pair<Int> {
            val vao = GL30.glGenVertexArrays()
            val vbo = GL15.glGenBuffers()
            GL30.glBindVertexArray(vao)
            GL15.glBindBuffer(GL15.GL_ARRAY_BUFFER, vbo)
            GL15.glBufferData(GL15.GL_ARRAY_BUFFER, Memory.convertVertices(vertices), GL15.GL_STATIC_DRAW)
            GL20.glEnableVertexAttribArray(0)
            GL20.glEnableVertexAttribArray(1)
            GL20.glEnableVertexAttribArray(2)
            GL20.glVertexAttribPointer(0, 3, GL11.GL_FLOAT, false, 32, 0)
            GL20.glVertexAttribPointer(1, 2, GL11.GL_FLOAT, false, 32, 12)
            GL20.glVertexAttribPointer(2, 3, GL11.GL_FLOAT, false, 32, 20)
            GL30.glBindVertexArray(0)
            GL15.glBindBuffer(GL15.GL_ARRAY_BUFFER, 0)
            GL20.glDisableVertexAttribArray(0)
            GL20.glDisableVertexAttribArray(1)
            GL20.glDisableVertexAttribArray(2)
            return Pair(vao, vbo)
        }

        fun generateScreenMesh(): Mesh {
            return Mesh(null, "Screen Mesh", null, arrayOf(
                    Vertex(Float3(-1.0f, -1.0f, 0.5f)), Vertex(Float3(-1.0f, 1.0f, 0.5f)), Vertex(Float3(1.0f, 1.0f, 0.5f)),
                    Vertex(Float3(1.0f, 1.0f, 0.5f)), Vertex(Float3(1.0f, -1.0f, 0.5f)), Vertex(Float3(-1.0f, -1.0f, 0.5f))
            ))
        }

        fun generateFrustumMesh(): Mesh {
            val frustumPoints = arrayOf(
                    Vertex(Float3(1f, 1f, 0f)),
                    Vertex(Float3(-1f, 1f, 0f)),
                    Vertex(Float3(1f, -1f, 0f)),
                    Vertex(Float3(-1f, -1f, 0f)),
                    Vertex(Float3(1f, 1f, 1f)),
                    Vertex(Float3(-1f, 1f, 1f)),
                    Vertex(Float3(1f, -1f, 1f)),
                    Vertex(Float3(-1f, -1f, 1f)))
            return Mesh(null, "Frustum Mesh", null, arrayOf(
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
                    frustumPoints[3], frustumPoints[7]))
        }

        fun generateGridMesh(size: Int): Mesh {
            val sideLen = size * 2 + 1
            val vertices = Array(sideLen * 4) { Vertex() }
            for (i in 0 until sideLen) {
                val pos = -size + i
                vertices[i * 4 + 0] = Vertex(Float3(pos.toFloat(), 0f, -size.toFloat()))
                vertices[i * 4 + 1] = Vertex(Float3(pos.toFloat(), 0f, +size.toFloat()))
                vertices[i * 4 + 2] = Vertex(Float3(-size.toFloat(), 0f, pos.toFloat()))
                vertices[i * 4 + 3] = Vertex(Float3(+size.toFloat(), 0f, pos.toFloat()))
            }
            return Mesh(null, "Grid Mesh", null, vertices)
        }
    }
}