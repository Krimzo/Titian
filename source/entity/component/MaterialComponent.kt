package entity.component

import entity.Entity
import entity.abs.EntityComponent
import entity.material.Material
import gui.helper.GUIPopup
import imgui.ImGui
import java.io.Serializable

class MaterialComponent(entity: Entity) : EntityComponent(entity), Serializable {
    var material: Material? = null

    fun use(callback: () -> Unit) {
        material?.use(callback)
    }

    override fun renderGUI() {
        ImGui.bulletText(material?.name ?: "None")
        GUIPopup.itemPopup("EditEntityMaterial") {
            if (ImGui.button("Remove entity.material")) {
                material = null
                GUIPopup.close()
            }
        }
    }
}