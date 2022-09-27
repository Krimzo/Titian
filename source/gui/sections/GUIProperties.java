package gui.sections;

import editor.*;
import entity.Entity;
import gui.*;
import imgui.*;
import imgui.flag.*;
import scene.*;

public final class GUIProperties extends GUISection implements GUIRenderable {
    public GUIProperties(Editor editor) {
        super(editor);
    }

    private Entity getSelectedEntity() {
        Scene scene = editor.getScene();
        if (scene != null) {
            return scene.selectedEntity;
        }
        return null;
    }

    @Override
    public void onGUIRender() {
        if (ImGui.begin("Properties", ImGuiWindowFlags.NoScrollbar)) {
            Entity selected = getSelectedEntity();
            if (selected != null) {
                selected.transformComponent.onGUIRender();
                selected.meshComponent.onGUIRender();
                selected.materialComponent.onGUIRender();
                selected.physicsComponent.onGUIRender();
            }
        }
        ImGui.end();
    }
}
