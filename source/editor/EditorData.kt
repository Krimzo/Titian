package editor

import entity.material.Material
import glparts.Mesh
import glparts.Shaders
import imgui.extension.imguizmo.flag.Mode
import named.NameHolder
import window.GLContext

class EditorData(context: GLContext, editor: Editor) {
    val frustumShaders: Shaders
    val unlitShaders: Shaders
    val defaultMesh: Mesh
    val frustumMesh: Mesh
    val defaultMaterial: Material
    var gameRunning: Boolean = false
    var wireframeState: Boolean = false
    var renderGrid: Boolean = true
    var gizmoOperation: Int = 0
    var gizmoMode: Int = Mode.WORLD

    init {
        frustumShaders = Shaders(editor.window.context, "shaders/Frustum.glsl")
        unlitShaders = Shaders(editor.window.context, "shaders/Unlit.glsl")
        defaultMesh = Mesh(NameHolder(), "Default Mesh", editor.window.context, "resource/meshes/cube.obj")
        frustumMesh = Mesh.generateFrustumMesh(context)
        defaultMaterial = Material(NameHolder(), "Default Material")
    }
}