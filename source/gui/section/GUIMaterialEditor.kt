package gui.section

import camera.PerspectiveCamera
import editor.Editor
import entity.Entity
import entity.material.Material
import glparts.Texture
import gui.abs.GUISection
import gui.helper.*
import imgui.ImGui
import imgui.flag.ImGuiTableFlags
import light.AmbientLight
import light.DirectionalLight
import math.Float2
import math.Int2
import scene.Scene
import utility.helper.FileHelper
import window.input.Input
import window.input.Mouse

class GUIMaterialEditor(editor: Editor) : GUISection(editor) {
    private var textInput: GUITextInput? = null
    private var selected: Any? = null
    private val camera: PerspectiveCamera
    private val startCameraInfo: Float2
    private val cameraInfo: Float2
    private val entity: Entity
    private val scene: Scene

    init {
        camera = PerspectiveCamera(null, null, editor)
        startCameraInfo = Float2()
        cameraInfo = Float2()
        camera.components.transform.position.z = 1f
        camera.sensitivity = 0.5f
        entity = Entity(null, null, editor)
        entity.components.mesh.mesh = editor.data.defaultMesh
        scene = Scene()
        scene.selected.ambientLight = AmbientLight(null, null, editor)
        scene.selected.directionalLight = DirectionalLight(null, null, editor)
        scene.add(entity)
    }

    private fun displayMaterials(scene: Scene?) {
        val it = scene!!.materials.iterator()
        while (it.hasNext()) {
            val material = it.next()
            val name = material.name
            if (ImGui.selectable(name, material === selected)) {
                selected = material
            }
            GUIPopup.itemPopup("EditMaterials$name") {
                if (ImGui.button("Rename")) {
                    textInput = GUITextInput(material.name) { newName: String ->
                        material.setName(newName)
                        textInput = null
                    }
                }
                if (ImGui.button("Delete", -1f, 0f)) {
                    it.remove()
                    material.eraseName()
                    if (material === selected) {
                        selected = null
                    }
                    for (entity in scene) {
                        if (entity.components.material.material === material) {
                            entity.components.material.material = null
                        }
                    }
                    GUIPopup.close()
                }
            }
        }
        GUIPopup.windowPopup("NewMaterial") {
            if (ImGui.button("New")) {
                textInput = GUITextInput("") { name: String? ->
                    scene.materials.add(Material(scene.names.material, name))
                    textInput = null
                }
            }
        }
    }

    private fun displayTextures(scene: Scene?) {
        var counter = 0
        val it = scene!!.textures.iterator()
        while (it.hasNext()) {
            val texture = it.next()
            if (ImGui.selectable(texture.name, texture === selected)) {
                selected = texture
            }
            GUIDragDrop.setData("TextureTransfer", texture)
            GUIPopup.itemPopup("EditTextures" + counter++) {
                if (ImGui.button("Rename")) {
                    textInput = GUITextInput(texture.name) { name: String ->
                        texture.setName(name)
                        textInput = null
                    }
                }
                if (ImGui.button("Delete", -1f, 0f)) {
                    it.remove()
                    texture.eraseName()
                    if (texture === selected) {
                        selected = null
                    }
                    for (entity in scene) {
                        entity.components.material.material.let { material ->
                            if (material?.colorMap === texture) {
                                material.colorMap = null
                            }
                            if (material?.normalMap === texture) {
                                material.normalMap = null
                            }
                            if (material?.roughnessMap === texture) {
                                material.roughnessMap = null
                            }
                        }
                    }
                    GUIPopup.close()
                }
            }
        }
    }

    private fun displayMaterialInfo(material: Material?) {
        val name = material!!.name
        ImGui.bulletText(name)
        GUIEdit.editColor3("Color##$name", material.color)
        material.colorBlend = GUIEdit.editFloat("Color blend##$name", material.colorBlend, 0.01f, 0f, 1f)
        material.roughness = GUIEdit.editFloat("Roughness##$name", material.roughness, 0.01f)
        if (ImGui.beginTable("##MaterialInfoTable", 2, ImGuiTableFlags.SizingStretchSame)) {
            ImGui.tableNextRow()
            ImGui.tableNextColumn()
            if (ImGui.selectable(material.colorMap?.name ?: "None")) {
                selected = material.colorMap
            }
            GUIDragDrop.getData("TextureTransfer") { texture: Any? ->
                material.colorMap = texture as Texture?
            }
            GUIPopup.itemPopup("EditMaterialColorMap$name") {
                if (ImGui.button("Remove")) {
                    material.colorMap = null
                    GUIPopup.close()
                }
            }
            ImGui.tableNextColumn()
            ImGui.text("Color map")
            ImGui.tableNextRow()
            ImGui.tableNextColumn()
            if (ImGui.selectable(material.roughnessMap?.name ?: "None")) {
                selected = material.roughnessMap
            }
            GUIDragDrop.getData("TextureTransfer") { texture: Any? ->
                material.roughnessMap = texture as Texture?
            }
            GUIPopup.itemPopup("EditMaterialRoughnessMap$name") {
                if (ImGui.button("Remove")) {
                    material.roughnessMap = null
                    GUIPopup.close()
                }
            }
            ImGui.tableNextColumn()
            ImGui.text("Roughness map")
            ImGui.tableNextRow()
            ImGui.tableNextColumn()
            if (ImGui.selectable(material.normalMap?.name ?: "None")) {
                selected = material.normalMap
            }
            GUIDragDrop.getData("TextureTransfer") { texture: Any? ->
                material.normalMap = texture as Texture?
            }
            GUIPopup.itemPopup("EditMaterialNormalMap$name") {
                if (ImGui.button("Remove")) {
                    material.normalMap = null
                    GUIPopup.close()
                }
            }
            ImGui.tableNextColumn()
            ImGui.text("Normal map")
            ImGui.endTable()
        }
    }

    private fun displayTextureInfo(texture: Texture?) {
        ImGui.bulletText(texture!!.name)
        GUIEdit.editFloat2("Size", Float2(texture.getSize()), 0f)
    }

    private fun updateCamera() {
        val position = camera.components.transform.position
        if (Input.isMousePressed(Mouse.Right)) {
            startCameraInfo.set(cameraInfo)
        }
        if (Input.isMouseDown(Mouse.Right)) {
            val dragDelta = ImGui.getMouseDragDelta(Mouse.Right)
            cameraInfo.x = startCameraInfo.x + dragDelta.x * camera.sensitivity
            cameraInfo.y = startCameraInfo.y + dragDelta.y * camera.sensitivity
            cameraInfo.y = Math.min(Math.max(cameraInfo.y, -85f), 85f)
            position.x = -Math.sin(Math.toRadians(cameraInfo.x.toDouble())).toFloat()
            position.z = Math.cos(Math.toRadians(cameraInfo.x.toDouble())).toFloat()
            position.y = Math.tan(Math.toRadians(cameraInfo.y.toDouble())).toFloat()
        }
        val cameraDistance = entity.components.mesh.mesh!!.maxRadius + 1
        position.set(position.normalize().multiply(cameraDistance))
        camera.setForward(position.negate())
        scene.selected.directionalLight!!.setDirection(camera.getForward())
    }

    private fun renderSelectedMaterial(viewportSize: Int2) {
        editor.editorRenderer.clear(camera)
        editor.window.context.setViewport(viewportSize)
        editor.editorRenderer.resize(viewportSize)
        camera.updateAspect(viewportSize)
        editor.window.context.setWireframe(editor.data.wireframeState)
        editor.editorRenderer.renderScene(scene, camera)
        editor.window.context.setWireframe(false)
        GUIDisplay.texture(editor.editorRenderer.renderBuffer.getColorMap(), viewportSize)
    }

    override fun renderGUI() {
        if (ImGui.begin("Material Editor")) {
            val availWidth = ImGui.getContentRegionAvailX()

            ImGui.columns(3, "MaterialEditorColumns", false)
            ImGui.setColumnWidth(ImGui.getColumnIndex(), availWidth * 0.25f)
            if (ImGui.beginChild("Materials")) {
                displayMaterials(editor.scene)
            }
            ImGui.endChild()

            ImGui.nextColumn()
            ImGui.setColumnWidth(ImGui.getColumnIndex(), availWidth * 0.25f)
            if (ImGui.beginChild("Textures")) {
                displayTextures(editor.scene)
            }
            ImGui.endChild()

            GUIDragDrop.getData("ImageFile") { path: Any? ->
                try {
                    val fileName = FileHelper.getNameWithoutExtension(path as String)
                    editor.scene.textures.add(Texture(editor.scene.names.texture, fileName, editor.window.context, path))
                } catch (ignored: Exception) {
                    println("Texture \"$path\" loading error!")
                }
            }
            var selectedMaterial: Material? = null
            var selectedTexture: Texture? = null

            selected?.let { selected ->
                if (selected.javaClass == Material::class.java) {
                    selectedMaterial = selected as Material?
                    entity.components.material.material = selectedMaterial
                }
                else if (selected.javaClass == Texture::class.java) {
                    selectedTexture = selected as Texture?
                }
            }
            ImGui.nextColumn()

            if (ImGui.beginChild("Texture View")) {
                val viewportSize = Int2(ImGui.getContentRegionAvailX().toInt(), ImGui.getContentRegionAvailY().toInt())
                if (selectedMaterial != null) {
                    updateCamera()
                    renderSelectedMaterial(viewportSize)
                }
                else if (selectedTexture != null) {
                    GUIDisplay.texture(selectedTexture, viewportSize)
                }
            }
            ImGui.endChild()

            if (ImGui.begin("Material/Texture")) {
                if (selectedMaterial != null) {
                    displayMaterialInfo(selectedMaterial)
                }
                else if (selectedTexture != null) {
                    displayTextureInfo(selectedTexture)
                }
            }
            ImGui.end()

            textInput?.update()
        }
        ImGui.end()
    }
}