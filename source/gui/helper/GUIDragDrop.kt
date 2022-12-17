package gui.helper

import glparts.Texture
import imgui.ImGui

object GUIDragDrop {
    fun setData(id: String, data: Any) {
        if (ImGui.beginDragDropSource()) {
            ImGui.setDragDropPayload(id, data)
            ImGui.endDragDropSource()
        }
    }

    fun setData(id: String, data: Any, texture: Texture?) {
        if (ImGui.beginDragDropSource()) {
            ImGui.setDragDropPayload(id, data)
            texture?.let {
                ImGui.image(it.buffer, 50f, 50f)
            }
            ImGui.endDragDropSource()
        }
    }

    fun getData(id: String, callback: (Any?) -> Unit) {
        if (ImGui.beginDragDropTarget()) {
            callback(ImGui.acceptDragDropPayload<Any>(id))
            ImGui.endDragDropTarget()
        }
    }
}