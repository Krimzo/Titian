package entity.component

import entity.Entity
import entity.abs.EntityComponent
import glparts.Mesh
import glparts.Shaders
import gui.helper.GUIPopup
import imgui.ImGui
import renderer.abs.Renderable
import java.io.Serializable

class MeshComponent(entity: Entity) : EntityComponent(entity), Renderable, Serializable {
    var mesh: Mesh? = null

    override fun gameRender(shaders: Shaders) {
        mesh?.renderTriangles(shaders)
    }

    override fun editorRender(shaders: Shaders) {
        // In case of future changes
        mesh?.renderTriangles(shaders)
    }

    override fun indexRender(shaders: Shaders) {
        // In case of future changes
        mesh?.renderTriangles(shaders)
    }

    override fun renderGUI() {
        ImGui.bulletText(mesh?.name ?: "None")
        GUIPopup.itemPopup("EditEntityMesh") {
            if (ImGui.button("Remove mesh")) {
                mesh = null
                GUIPopup.close()
            }
        }
    }
}