package gui.section

import editor.Editor
import glparts.Mesh
import glparts.Shaders
import gui.abs.GUISection
import gui.helper.GUIDisplay
import gui.helper.GUIDragDrop
import gui.helper.GUIEdit
import gui.helper.GUIStyle
import imgui.ImGui
import imgui.extension.imguizmo.ImGuizmo
import imgui.extension.imguizmo.flag.Mode
import imgui.extension.imguizmo.flag.Operation
import imgui.flag.ImGuiMouseButton
import imgui.flag.ImGuiStyleVar
import imgui.flag.ImGuiWindowFlags
import math.*
import scene.Scene
import window.input.Input
import window.input.Key
import window.input.Mouse

class GUIViewport(editor: Editor) : GUISection(editor) {
    private var viewportPosition: Int2 = Int2()
    private var viewportSize: Int2 = Int2()
    private var gridMesh: Mesh
    private var gridShaders: Shaders
    private val snap: Array<FloatArray>

    init {
        gridMesh = Mesh.generateGridMesh(editor.window.context, 50)
        gridShaders = Shaders(editor.window.context, "shaders/Grid.glsl")
        snap = arrayOf(floatArrayOf(0.1f, 0.1f, 0.1f), floatArrayOf(30f, 30f, 30f), floatArrayOf(1f, 1f, 1f), floatArrayOf(0f, 0f, 0f))
    }

    private fun updateViewport() {
        val tabSize = ImGui.getWindowContentRegionMinY().toInt()
        viewportPosition = Int2(ImGui.getWindowPosX().toInt(), ImGui.getWindowPosY().toInt() + tabSize)
        viewportSize = Int2(ImGui.getWindowWidth().toInt(), ImGui.getWindowHeight().toInt() - tabSize)
    }

    private fun renderViewportInfo() {
        ImGui.pushStyleVar(ImGuiStyleVar.WindowPadding, 14f, 14f)
        if (ImGui.begin("Viewport Info")) {
            ImGui.bulletText("Camera")
            GUIEdit.editFloat3("Position", editor.camera.components.transform.position, 0.1f)

            val data = editor.camera.getForward().array
            if (ImGui.dragFloat3("Forward", data, 0.05f)) {
                editor.camera.setForward(Float3(data))
            }
        }
        ImGui.end()
        ImGui.popStyleVar()
    }

    private fun updateCamera() {
        editor.camera.useDefaultMovement(Mouse.Right, Key.W, Key.S, Key.D, Key.A, Key.E, Key.Q, 2f,
            editor.window.size, editor.timer.getDeltaT()
        )
    }

    private fun updateGizmoState() {
        if (ImGui.isKeyPressed(Key.Num1)) {
            editor.data.gizmoOperation = if (editor.data.gizmoOperation != Operation.SCALE) Operation.SCALE else 0
        }

        if (ImGui.isKeyPressed(Key.Num2)) {
            editor.data.gizmoOperation = if (editor.data.gizmoOperation != Operation.ROTATE) Operation.ROTATE else 0
        }

        if (ImGui.isKeyPressed(Key.Num3)) {
            editor.data.gizmoOperation = if (editor.data.gizmoOperation != Operation.TRANSLATE) Operation.TRANSLATE else 0
        }

        if (ImGui.isKeyPressed(Key.Num4)) {
            editor.data.gizmoMode = if (editor.data.gizmoMode == Mode.WORLD) Mode.LOCAL else Mode.WORLD
        }
    }

    private fun objectSelection() {
        if (ImGuizmo.isOver()) {
            return
        }

        val viewportRect = Float4(
            viewportPosition.x.toFloat(), viewportPosition.y.toFloat(), (viewportPosition.x + viewportSize.x).toFloat(), (viewportPosition.y + viewportSize.y).toFloat()
        )

        if (ImGui.isMouseHoveringRect(viewportRect.x, viewportRect.y, viewportRect.z, viewportRect.w) && ImGui.isMouseClicked(ImGuiMouseButton.Left)) {
            val mousePosition = Float2(ImGui.getMousePosX() - viewportPosition.x, ImGui.getMousePosY() - viewportPosition.y)
            val objectIndex = editor.editorRenderer.indexBuffer.getPixel(Int2(mousePosition)).x.toInt() - 1
            editor.scene.selected.entity = if (objectIndex >= 0) editor.scene[objectIndex] else null
        }
    }

    private fun renderScene() {
        editor.editorRenderer.clear(editor.camera)
        editor.window.context.setViewport(viewportSize)
        editor.editorRenderer.resize(viewportSize)
        editor.camera.updateAspect(viewportSize)

        if (editor.data.renderGrid) {
            editor.editorRenderer.renderBuffer.use {
                var position = editor.camera.components.transform.position
                position = Float3(position.x.toInt().toFloat(), 0f, position.z.toInt().toFloat())
                gridShaders.setUniform("W", Mat4.translation(position))
                gridShaders.setUniform("VP", editor.camera.matrix())
                gridMesh.renderLines(gridShaders)
            }
        }

        val selectedIndex = editor.scene.indexOf(editor.scene.selected.entity)
        editor.window.context.setWireframe(editor.data.wireframeState)
        editor.editorRenderer.renderScene(editor.scene, editor.camera)
        editor.editorRenderer.renderIndices(editor.scene, editor.camera)
        editor.window.context.setWireframe(false)

        if (selectedIndex >= 0) {
            editor.editorRenderer.renderOutline(Float2(viewportSize), GUIStyle.special, selectedIndex)
        }
    }

    private fun renderGizmos() {
        ImGuizmo.setEnabled(true)
        ImGuizmo.setDrawList()
        ImGuizmo.setRect(viewportPosition.x.toFloat(), viewportPosition.y.toFloat(), viewportSize.x.toFloat(), viewportSize.y.toFloat())

        val selected = editor.scene.selected.entity
        if (selected == null || editor.data.gizmoOperation == 0) {
            val ignored = FloatArray(16)
            ImGuizmo.manipulate(ignored, ignored, ignored, editor.data.gizmoOperation, editor.data.gizmoMode)
            return
        }

        val transform = selected.components.transform
        val viewMatrix = editor.camera.viewMatrix().transpose()
        val projectionMatrix = editor.camera.projectionMatrix().transpose()
        val transformMatrix = transform.matrix().transpose()
        var snap = snap[3]
        if (Input.isShiftDown) {
            when (editor.data.gizmoOperation) {
                Operation.SCALE -> snap = this.snap[0]
                Operation.ROTATE -> snap = this.snap[1]
                Operation.TRANSLATE -> snap = this.snap[2]
            }
        }

        val result = FloatArray(16)
        ImGuizmo.manipulate(viewMatrix.data, projectionMatrix.data, transformMatrix.data, result,
            editor.data.gizmoOperation, editor.data.gizmoMode, snap, this.snap[3], this.snap[3]
        )

        if (ImGuizmo.isUsing()) {
            val decomposed = Array(3) { FloatArray(3) }
            ImGuizmo.decomposeMatrixToComponents(transformMatrix.data, decomposed[2], decomposed[1], decomposed[0])
            transform.scale = Float3(decomposed[0])
            transform.position = Float3(decomposed[2])

            ImGuizmo.decomposeMatrixToComponents(result, decomposed[2], decomposed[1], decomposed[0])
            transform.rotation = transform.rotation + Float3(decomposed[1])
        }
    }

    override fun renderGUI() {
        ImGui.pushStyleVar(ImGuiStyleVar.WindowPadding, 0f, 0f)
        ImGui.setNextWindowBgAlpha(1f)
        if (ImGui.begin("Viewport", ImGuiWindowFlags.NoScrollbar)) {
            updateViewport()
            renderViewportInfo()

            if (ImGui.isWindowFocused()) {
                updateCamera()
                updateGizmoState()
                objectSelection()
            }

            renderScene()
            GUIDisplay.texture(editor.editorRenderer.renderBuffer.getColorMap(), viewportSize)

            GUIDragDrop.getData("SceneFile") { path: Any ->
                if (path is String) {
                    Scene.fromFile(path, editor)?.let {
                        editor.scene = it
                        println("Scene \"$path\" loaded")
                    }
                }
            }

            renderGizmos()
        }
        ImGui.end()
        ImGui.popStyleVar()
    }
}