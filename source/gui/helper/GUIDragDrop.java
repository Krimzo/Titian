package gui.helper;

import callback.ObjectCallback;
import glparts.Texture;
import imgui.ImGui;
import utility.Instance;

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
            if (Instance.isValid(object)) {
                callback.method(object);
            }
            ImGui.endDragDropTarget();
        }
    }
}
