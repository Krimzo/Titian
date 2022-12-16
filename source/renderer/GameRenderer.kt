package renderer

import camera.abs.Camera
import glparts.FrameBuffer
import glparts.Shaders
import math.Float3
import math.Float4
import math.Int2
import renderer.abs.Renderable
import renderer.abs.Renderer
import scene.Scene
import window.GLContext

open class GameRenderer(context: GLContext?, size: Int2) : Renderer() {
    val renderBuffer: FrameBuffer
    protected val renderShaders: Shaders

    init {
        renderBuffer = FrameBuffer(context, size)
        renderShaders = Shaders(context, "shaders/Render.glsl")
    }

    override fun resize(size: Int2) {
        renderBuffer.resize(size)
    }

    override fun clear(camera: Camera?) {
        val color = camera?.background ?: Float3()
        renderBuffer.context!!.setClearColor(Float4(color, 1f))
        renderBuffer.clear()
    }

    override fun renderRenderable(renderable: Renderable) {
        renderable.gameRender(renderShaders)
    }

    override fun renderScene(scene: Scene?, camera: Camera?) {
        renderBuffer.use {
            renderShaders.setUniform("VP", camera!!.matrix())
            renderShaders.setUniform("ambientColor", scene?.selected?.ambientLight?.getColor() ?: Float3())
            renderShaders.setUniform("sunDirection", scene?.selected?.directionalLight?.direction ?: Float3())
            renderShaders.setUniform("sunColor", scene?.selected?.directionalLight?.getColor() ?: Float3())
            scene?.let {
                for (obj in it) {
                    renderRenderable(obj)
                }
            }
        }
    }
}