package camera

import camera.abs.Camera
import editor.Editor
import glparts.Shaders
import gui.helper.GUIEdit
import gui.helper.GUIStyle
import math.Float3
import math.Int2
import math.Mat4
import named.NameHolder
import java.io.Serializable

class PerspectiveCamera(holder: NameHolder, name: String, editor: Editor) : Camera(holder, name, editor), Serializable {
    var aspect: Float = 1.78f
    var fov: Float = 90f

    fun updateAspect(frameSize: Int2) {
        aspect = frameSize.x.toFloat() / frameSize.y
    }

    override fun projectionMatrix(): Mat4 {
        return Mat4.perspective(fov, aspect, near, far)
    }

    override fun matrix(): Mat4 {
        return projectionMatrix() * viewMatrix()
    }

    override fun renderInfoGUI(editor: Editor) {
        super.renderInfoGUI(editor)
        aspect = GUIEdit.editFloat("Aspect", aspect, 0.01f)
        fov = GUIEdit.editFloat("FOV", fov, 0.1f)
    }

    override fun editorRender(shaders: Shaders) {
        editor.data.frustumShaders.setUniform("iVP", matrix().inv())
        editor.data.frustumShaders.setUniform("VP", editor.camera.matrix())
        editor.data.frustumShaders.setUniform("color", if (this === editor.scene.selected.entity) GUIStyle.special else Float3(1f))
        editor.data.frustumMesh.renderLines(editor.data.frustumShaders)
        super.editorRender(shaders)
    }
}