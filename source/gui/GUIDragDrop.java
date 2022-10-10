package gui;

import callback.ObjectCallback;
import glparts.Texture;
import imgui.ImGui;

public final class GUIDragDrop {
    private GUIDragDrop() {}

    public static void setData(String id, Object data) {
        if (ImGui.beginDragDropSource()) {
            ImGui.setDragDropPayload(id, data);
            ImGui.endDragDropSource();
        }
    }

    public static void setData(String id, Object data, Texture texture) {
        if (ImGui.beginDragDropSource()) {
            ImGui.setDragDropPayload(id, data);
            ImGui.image(texture.getBuffer(), 50, 50);
            ImGui.endDragDropSource();
        }
    }

    public static void getData(String id, ObjectCallback callback) {
        if (ImGui.beginDragDropTarget()) {
            Object object = ImGui.acceptDragDropPayload(id);
            if (object != null) {
                callback.method(object);
            }
            ImGui.endDragDropTarget();
        }
    }
}
