package editor

import entity.material.Material
import glparts.Mesh
import glparts.Shaders
import imgui.extension.imguizmo.flag.Mode

class EditorData(editor: Editor) {
    val frustumShaders: Shaders
    val unlitShaders: Shaders
    val defaultMesh: Mesh
    val frustumMesh: Mesh
    val defaultMaterial: Material
    var gameRunning = false
    var wireframeState = false
    var renderGrid = true
    var gizmoOperation = 0
    var gizmoMode = Mode.WORLD

    init {
        frustumShaders = Shaders(editor.window.context, "shaders/Frustum.glsl")
        unlitShaders = Shaders(editor.window.context, "shaders/Unlit.glsl")
        defaultMesh = Mesh(null, "Default Mesh", editor.window.context, "resource/meshes/cube.obj")
        frustumMesh = Mesh.Companion.generateFrustumMesh()
        defaultMaterial = Material(null, "Default Material")
    }
}