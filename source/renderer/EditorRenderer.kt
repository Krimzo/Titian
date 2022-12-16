package renderer

import camera.abs.Camera
import glparts.FrameBuffer
import glparts.Mesh
import glparts.Shaders
import math.Float2
import math.Float3
import math.Float4
import math.Int2
import org.lwjgl.opengl.GL12
import renderer.abs.Renderable
import scene.Scene
import window.GLContext

class EditorRenderer(context: GLContext?, size: Int2) : GameRenderer(context, size) {
    val indexBuffer: FrameBuffer
    private val indexShaders: Shaders
    private val outlineShaders: Shaders
    private val screenMesh: Mesh

    init {
        indexBuffer = FrameBuffer(context, size)
        indexBuffer.getColorMap().setWrap(GL12.GL_CLAMP_TO_EDGE, GL12.GL_CLAMP_TO_EDGE)
        indexShaders = Shaders(context, "shaders/Index.glsl")
        outlineShaders = Shaders(context, "shaders/Outline.glsl")
        screenMesh = Mesh.generateScreenMesh()
    }

    override fun resize(size: Int2) {
        super.resize(size)
        indexBuffer.resize(size)
    }

    override fun renderRenderable(renderable: Renderable) {
        renderable.editorRender(renderShaders)
    }

    fun renderIndices(scene: Scene?, camera: Camera?) {
        indexBuffer.context!!.setClearColor(Float4(0f))
        indexBuffer.clear()
        indexBuffer.use {
            indexShaders.setUniform("VP", camera!!.matrix())
            for (i in scene!!.indices) {
                indexShaders.setUniform("index", i + 1)
                scene[i]!!.indexRender(indexShaders)
            }
        }
    }

    fun renderOutline(frameSize: Float2, outlineColor: Float3?, objectIndex: Int) {
        indexBuffer.getColorMap().use(0) {
            renderBuffer.context!!.setDepthTest(false)
            renderBuffer.use {
                outlineShaders.setUniform("indexMap", 0)
                outlineShaders.setUniform("outlineColor", Float4(outlineColor, 1f))
                outlineShaders.setUniform("frameSize", frameSize)
                outlineShaders.setUniform("outlineThickness", 1)
                outlineShaders.setUniform("selectedIndex", objectIndex + 1)
                screenMesh.renderTriangles(outlineShaders)
            }
            renderBuffer.context.setDepthTest(true)
        }
    }
}