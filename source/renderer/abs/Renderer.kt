package renderer.abs

import camera.abs.Camera
import math.Int2
import scene.Scene

abstract class Renderer {
    abstract fun resize(size: Int2)
    abstract fun clear(camera: Camera?)
    protected abstract fun renderRenderable(renderable: Renderable)
    abstract fun renderScene(scene: Scene?, camera: Camera?)
}