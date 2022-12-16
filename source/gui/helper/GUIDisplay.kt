package gui.helper

import glparts.DepthTexture
import glparts.Texture
import imgui.ImGui
import math.Float2
import math.Int2

object GUIDisplay {
    fun texture(texture: Texture?, size: Int2?) {
        texture(texture, Float2(size))
    }

    fun texture(texture: DepthTexture?, size: Int2?) {
        texture(texture, Float2(size))
    }

    fun texture(texture: Texture?, size: Float2) {
        texture(texture!!.buffer, size.x, size.y)
    }

    fun texture(texture: DepthTexture?, size: Float2) {
        texture(texture!!.buffer, size.x, size.y)
    }

    fun texture(texture: Int, width: Float, height: Float) {
        ImGui.image(texture, width, height, 0f, 1f, 1f, 0f)
    }
}