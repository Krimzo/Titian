package gui.section

import editor.Editor
import entity.Entity
import gui.abs.GUISection
import gui.helper.GUIDragDrop
import gui.helper.GUIPopup
import imgui.ImGui
import imgui.flag.ImGuiTreeNodeFlags
import script.Script

class GUIProperties(editor: Editor) : GUISection(editor) {
    private fun renderTransform(selected: Entity) {
        selected.components.transform.renderGUI()
    }

    private fun renderMesh(selected: Entity) {
        selected.components.mesh.renderGUI()
        if (ImGui.beginListBox("##Loaded meshes", -1f, 0f)) {
            for (mesh in editor.scene.meshes) {
                if (ImGui.selectable(mesh.name, selected.components.mesh.mesh === mesh)) {
                    selected.components.mesh.mesh = mesh
                }
            }
            ImGui.endListBox()
        }
    }

    private fun renderMaterial(selected: Entity) {
        selected.components.material.renderGUI()
        if (ImGui.beginListBox("##Loaded materials", -1f, 0f)) {
            for (material in editor.scene.materials) {
                if (ImGui.selectable(material.name, selected.components.material.material === material)) {
                    selected.components.material.material = material
                }
            }
            ImGui.endListBox()
        }
    }

    private fun renderPhysics(selected: Entity) {
        selected.components.physics.renderGUI()
    }

    private fun renderScripts(selected: Entity) {
        selected.components.script.renderGUI()
        val contentMin = ImGui.getWindowContentRegionMin()
        val contentMax = ImGui.getWindowContentRegionMax()
        ImGui.dummy(contentMax.x - contentMin.x, contentMax.y - contentMin.y)

        GUIDragDrop.getData("ScriptFile") { path: Any? ->
            if (path is String) {
                selected.components.script.scripts.add(Script(path, selected))
            }
        }

        GUIPopup.itemPopup("Scripts") {
            if (ImGui.button("Reload")) {
                selected.components.script.reload()
                GUIPopup.close()
            }
        }
    }

    override fun renderGUI() {
        val selected = editor.scene.selected.entity
        if (ImGui.begin("Properties") && selected != null) {
            if (ImGui.collapsingHeader("Info", ImGuiTreeNodeFlags.DefaultOpen)) {
                selected.renderInfoGUI(editor)
                ImGui.separator()
            }

            if (ImGui.collapsingHeader("Transform", ImGuiTreeNodeFlags.DefaultOpen)) {
                renderTransform(selected)
                ImGui.separator()
            }

            if (ImGui.collapsingHeader("Mesh", ImGuiTreeNodeFlags.DefaultOpen)) {
                renderMesh(selected)
                ImGui.separator()
            }

            if (ImGui.collapsingHeader("Material", ImGuiTreeNodeFlags.DefaultOpen)) {
                renderMaterial(selected)
                ImGui.separator()
            }

            if (ImGui.collapsingHeader("Physics", ImGuiTreeNodeFlags.DefaultOpen)) {
                renderPhysics(selected)
                ImGui.separator()
            }

            if (ImGui.collapsingHeader("Scripts", ImGuiTreeNodeFlags.DefaultOpen)) {
                renderScripts(selected)
            }
        }
        ImGui.end()
    }
}