package gui.section

import camera.OrthographicCamera
import camera.PerspectiveCamera
import editor.Editor
import entity.Entity
import gui.abs.GUISection
import gui.helper.GUIPopup
import gui.helper.GUITextInput
import imgui.ImGui
import imgui.flag.ImGuiWindowFlags
import light.AmbientLight
import light.DirectionalLight
import light.PositionalLight
import window.input.Input
import java.util.concurrent.atomic.AtomicInteger

class GUIScene(editor: Editor) : GUISection(editor) {
    private var textInput: GUITextInput? = null

    private fun addEntity() {
        if (ImGui.button("Entity", -1f, 0f)) {
            val entity = Entity(editor.scene.names.entity, "Entity", editor)
            editor.scene.add(entity)

            // (Mesh, Material) bind after .add() call so defaults don't get added to the scene list
            entity.components.mesh.mesh = editor.data.defaultMesh
            entity.components.material.material = editor.data.defaultMaterial
            GUIPopup.close()
        }
    }

    private fun addCamera() {
        if (ImGui.beginMenu("Camera")) {
            if (ImGui.button("Perspective", -1f, 0f)) {
                val camera: Entity = PerspectiveCamera(editor.scene.names.entity, "Perspective camera", editor)
                editor.scene.add(camera)
                GUIPopup.close()
            }
            if (ImGui.button("Orthographic")) {
                val camera: Entity = OrthographicCamera(editor.scene.names.entity, "Orthographic camera", editor)
                editor.scene.add(camera)
                GUIPopup.close()
            }
            ImGui.endMenu()
        }
    }

    private fun addLight() {
        if (ImGui.beginMenu("Light")) {
            if (ImGui.button("Ambient", -1f, 0f)) {
                val light: Entity = AmbientLight(editor.scene.names.entity, "Ambient Light", editor)
                editor.scene.add(light)
                GUIPopup.close()
            }
            if (ImGui.button("Directional")) {
                val camera: Entity = DirectionalLight(editor.scene.names.entity, "Directional Light", editor)
                editor.scene.add(camera)
                GUIPopup.close()
            }
            if (ImGui.button("Positional", -1f, 0f)) {
                val camera: Entity = PositionalLight(editor.scene.names.entity, "Positional Light", editor)
                editor.scene.add(camera)
                GUIPopup.close()
            }
            ImGui.endMenu()
        }
    }

    private fun windowPopup() {
        if (ImGui.beginMenu("New")) {
            addEntity()
            addCamera()
            addLight()
            ImGui.endMenu()
        }
    }

    override fun renderGUI() {
        if (ImGui.begin("Scene", ImGuiWindowFlags.NoScrollbar)) {
            GUIPopup.windowPopup("SceneWindow") { windowPopup() }
            val i = AtomicInteger()
            while (i.get() < editor.scene.size) {
                val entity = editor.scene[i.get()]
                if (ImGui.selectable(entity.name, entity === editor.scene.selected.entity)) {
                    editor.scene.selected.entity = entity
                }
                GUIPopup.itemPopup("EditEntity$i") {
                    if (Input.isShiftDown) {
                        return@itemPopup
                    }

                    if (ImGui.button("Rename")) {
                        textInput = GUITextInput(entity.name) { name: String ->
                            entity.setName(name)
                            textInput = null
                        }
                    }

                    if (ImGui.button("Delete", -1f, 0f)) {
                        val removed = editor.scene.removeAt(i.getAndDecrement())
                        removed.eraseName()
                        if (removed === editor.scene.selected.entity) {
                            editor.scene.selected.entity = null
                        }
                        if (removed === editor.scene.selected.camera) {
                            editor.scene.selected.camera = null
                        }
                        GUIPopup.close()
                    }
                }
                i.getAndIncrement()
            }

            textInput?.update()
        }
        ImGui.end()
    }
}