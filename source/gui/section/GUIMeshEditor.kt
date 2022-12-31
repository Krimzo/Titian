package gui.section

import camera.PerspectiveCamera
import editor.Editor
import entity.Entity
import entity.material.Material
import glparts.Mesh
import glparts.Texture
import gui.abs.GUISection
import gui.helper.*
import imgui.ImGui
import imgui.flag.ImGuiWindowFlags
import light.AmbientLight
import light.DirectionalLight
import math.Float2
import math.Int2
import math.normalize
import math.toRadians
import named.NameHolder
import scene.Scene
import utility.helper.FileHelper
import window.input.Input
import window.input.Mouse
import kotlin.math.cos
import kotlin.math.sin
import kotlin.math.tan

class GUIMeshEditor(editor: Editor) : GUISection(editor) {
    private var textInput: GUITextInput? = null
    private val camera: PerspectiveCamera
    private val startCameraInfo: Float2
    private val cameraInfo: Float2
    private val entity: Entity
    private val scene: Scene

    init {
        val holder = NameHolder()
        camera = PerspectiveCamera(holder, "", editor)
        camera.components.transform.position.z = 1f
        camera.sensitivity = 0.5f
        startCameraInfo = Float2()
        cameraInfo = Float2()

        val material = Material(holder, "Mesh Editor Material")
        material.colorMap = Texture(holder, "Mesh Editor Texture", editor.window.context, Int2(1, 1), byteArrayOf(200.toByte(), 200.toByte(), 200.toByte(), 0xFF.toByte()))

        entity = Entity(holder, "", editor)
        entity.components.material.material = material

        scene = Scene()
        scene.selected.ambientLight = AmbientLight(holder, "", editor)
        scene.selected.directionalLight = DirectionalLight(holder, "", editor)
        scene.add(entity)
    }

    private fun displayMeshes(scene: Scene) {
        var counter = 0
        val iter = scene.meshes.iterator()
        val mc = entity.components.mesh

        while (iter.hasNext()) {
            val mesh = iter.next()
            if (ImGui.selectable(mesh.name, mesh === mc.mesh)) {
                mc.mesh = mesh
            }

            GUIPopup.itemPopup("EditMeshes" + counter++) {
                if (ImGui.button("Rename")) {
                    textInput = GUITextInput(mesh.name) { name: String ->
                        mesh.setName(name)
                        textInput = null
                    }
                }

                if (ImGui.button("Delete", -1f, 0f)) {
                    iter.remove()
                    mesh.eraseName()
                    if (mesh === mc.mesh) {
                        mc.mesh = null
                    }

                    for (entity in scene) {
                        if (entity.components.mesh.mesh === mesh) {
                            entity.components.mesh.mesh = null
                        }
                    }
                    GUIPopup.close()
                }
            }
        }
    }

    private fun updateCamera() {
        if (Input.isMousePressed(Mouse.Right)) {
            startCameraInfo.x = cameraInfo.x
            startCameraInfo.y = cameraInfo.y
        }

        if (Input.isMouseDown(Mouse.Right)) {
            val dragDelta = ImGui.getMouseDragDelta(Mouse.Right)
            cameraInfo.x = startCameraInfo.x + dragDelta.x * camera.sensitivity
            cameraInfo.y = startCameraInfo.y + dragDelta.y * camera.sensitivity
            cameraInfo.y = cameraInfo.y.coerceAtLeast(-85f).coerceAtMost(85f)

            val position = camera.components.transform.position
            position.x = -sin(toRadians(cameraInfo.x))
            position.z = cos(toRadians(cameraInfo.x))
            position.y = tan(toRadians(cameraInfo.y))
        }

        val cameraDistance = (entity.components.mesh.mesh?.maxRadius ?: 0f) + 1f
        camera.components.transform.position = normalize(camera.components.transform.position) * cameraDistance
        camera.setForward(-camera.components.transform.position)
        scene.selected.directionalLight?.setDirection(camera.getForward())
    }

    private fun renderSelectedMesh(viewportSize: Int2) {
        editor.editorRenderer.clear(camera)
        editor.window.context.setViewport(viewportSize)
        editor.editorRenderer.resize(viewportSize)
        camera.updateAspect(viewportSize)
        editor.window.context.setWireframe(editor.data.wireframeState)
        editor.editorRenderer.renderScene(scene, camera)
        editor.window.context.setWireframe(false)
    }

    override fun renderGUI() {
        if (ImGui.begin("Mesh Editor", ImGuiWindowFlags.NoScrollbar)) {
            val availWidth = ImGui.getContentRegionAvailX()
            ImGui.columns(2, "MeshEditorColumns", false)

            ImGui.setColumnWidth(ImGui.getColumnIndex(), availWidth * 0.25f)
            if (ImGui.beginChild("Meshes")) {
                displayMeshes(editor.scene)
            }
            ImGui.endChild()

            GUIDragDrop.getData("MeshFile") { path: Any ->
                try {
                    FileHelper.getNameWithoutExtension(path as String)?.let {
                        editor.scene.meshes.add(Mesh(editor.scene.names.mesh, it, editor.window.context, path))
                    }
                }
                catch (ignored: Exception) {
                    println("Mesh \"$path\" loading error!")
                }
            }
            ImGui.nextColumn()

            if (ImGui.beginChild("Mesh View") && entity.components.mesh.mesh != null) {
                val viewportSize = Int2(ImGui.getContentRegionAvailX().toInt(), ImGui.getContentRegionAvailY().toInt())
                updateCamera()
                renderSelectedMesh(viewportSize)
                GUIDisplay.texture(editor.editorRenderer.renderBuffer.getColorMap(), viewportSize)
            }
            ImGui.endChild()

            if (ImGui.begin("Mesh")) {
                entity.components.mesh.mesh?.let {
                    ImGui.bulletText(it.name)
                    GUIEdit.editInt("Vertex count", it.getVertexCount(), 0)
                    GUIEdit.editFloat("Max radius", it.maxRadius, 0f)
                }
            }
            ImGui.end()

            textInput?.update()
        }
        ImGui.end()
    }
}