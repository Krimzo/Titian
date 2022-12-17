package camera

import camera.abs.Camera
import editor.Editor
import glparts.Shaders
import gui.helper.GUIEdit
import gui.helper.GUIStyle
import math.Float3
import math.Mat4
import named.NameHolder
import java.io.Serializable

class OrthographicCamera(holder: NameHolder, name: String, editor: Editor) : Camera(holder, name, editor), Serializable {
    var width = 160f
    var height = 90f

    override fun projectionMatrix(): Mat4 {
        return Mat4.orthographic(width, height, near, far)
    }

    override fun matrix(): Mat4 {
        return projectionMatrix() * viewMatrix()
    }

    override fun renderInfoGUI(editor: Editor) {
        super.renderInfoGUI(editor)
        width = GUIEdit.editFloat("Width", width, 0.1f)
        height = GUIEdit.editFloat("Height", height, 0.1f)
    }

    override fun editorRender(shaders: Shaders) {
        editor.data.frustumShaders.setUniform("iVP", matrix().inv())
        editor.data.frustumShaders.setUniform("VP", editor.camera.matrix())
        editor.data.frustumShaders.setUniform("color", if (this === editor.scene.selected.entity) GUIStyle.special else Float3(1f))
        editor.data.frustumMesh.renderLines(editor.data.frustumShaders)
        super.editorRender(shaders)
    }
}