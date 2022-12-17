package entity.material

import glparts.Shaders
import glparts.Texture
import math.Float3
import named.NameHolder
import named.Named
import java.io.Serializable

class Material(holder: NameHolder, name: String) : Named(holder, name), Serializable {
    var colorBlend: Float = 0f
    var roughness: Float = 0.5f

    var color: Float3 = Float3(1f)

    var colorMap: Texture? = null
    var roughnessMap: Texture? = null
    var normalMap: Texture? = null

    fun use(callback: () -> Unit) {
        val textures = IntArray(3)
        textures[0] = colorMap?.buffer ?: 0
        textures[1] = normalMap?.buffer ?: 0
        textures[2] = roughnessMap?.buffer ?: 0
        Texture.use(textures, callback)
    }

    fun updateUniforms(shaders: Shaders) {
        shaders.setUniform("color", color)
        shaders.setUniform("colorMap", 0)
        shaders.setUniform("colorBlend", colorBlend)
    }
}