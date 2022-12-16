package renderer.abs

import glparts.Shaders

interface Renderable {
    fun gameRender(shaders: Shaders)
    fun editorRender(shaders: Shaders)
    fun indexRender(shaders: Shaders)
}