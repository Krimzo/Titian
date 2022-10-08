package gui.section;

import camera.OrthographicCamera;
import camera.PerspectiveCamera;
import editor.Editor;
import entity.Entity;
import gui.abs.GUISection;
import imgui.ImGui;
import imgui.flag.ImGuiInputTextFlags;
import imgui.flag.ImGuiPopupFlags;
import imgui.flag.ImGuiWindowFlags;
import imgui.type.ImString;

public final class GUIScene extends GUISection {
    private static class RenameData {
        public Entity entity;
        public ImString name;

        public RenameData(Entity entity) {
            this.entity = entity;
            name = new ImString(entity.getName());
        }

        public boolean updateName() {
            String name = this.name.toString();

            if (name.length() > 0) {
                entity.setName(name);
                return true;
            }

            return false;
        }
    }

    private RenameData renameData = null;

    public GUIScene(Editor editor) {
        super(editor);
    }

    private void windowPopup() {
        if (ImGui.beginMenu("New")) {
            if (ImGui.button("Entity", -1, 0)) {
                Entity entity = new Entity(editor.scene.entityNames, "Entity", editor);
                entity.meshComponent.mesh = editor.data.defaultMesh;
                entity.materialComponent.material = editor.data.defaultMaterial;
                editor.scene.addUnsaved(entity);

                ImGui.closeCurrentPopup();
            }

            if (ImGui.beginMenu("Camera")) {
                if (ImGui.button("Perspective", -1, 0)) {
                    PerspectiveCamera camera = new PerspectiveCamera(editor.scene.entityNames, "Perspective camera", editor);
                    editor.scene.add(camera);

                    ImGui.closeCurrentPopup();
                }

                if (ImGui.button("Orthographic")) {
                    OrthographicCamera camera = new OrthographicCamera(editor.scene.entityNames, "Orthographic camera", editor);
                    editor.scene.add(camera);

                    ImGui.closeCurrentPopup();
                }

                ImGui.endMenu();
            }

            ImGui.endMenu();
        }
    }

    @Override
    public void renderGUI() {
        if (ImGui.begin("Scene", ImGuiWindowFlags.NoScrollbar) && editor.scene != null) {
            if (ImGui.beginPopupContextWindow()) {
                windowPopup();
                ImGui.endPopup();
            }

            for (int i = 0; i < editor.scene.size(); i++) {
                Entity entity = editor.scene.get(i);

                if (ImGui.selectable(entity.getName(), entity == editor.scene.selectedEntity)) {
                    editor.scene.selectedEntity = entity;
                }

                if (!editor.window.keyboard.shift.getState() && ImGui.beginPopupContextItem("EditEntity" + i)) {
                    if (ImGui.button("Rename")) {
                        renameData = new RenameData(entity);
                    }

                    if (ImGui.button("Delete", -1, 0)) {
                        Entity removed = editor.scene.remove(i--);

                        if (removed == editor.scene.selectedEntity) {
                            editor.scene.selectedEntity = null;
                        }

                        if (removed == editor.scene.mainCamera) {
                            editor.scene.mainCamera = null;
                        }

                        ImGui.closeCurrentPopup();
                    }

                    ImGui.endPopup();
                }
            }

            if (renameData != null) {
                if (ImGui.begin("Rename entity", ImGuiWindowFlags.NoScrollbar | ImGuiWindowFlags.AlwaysAutoResize)) {
                    if (ImGui.inputText("New name", renameData.name, ImGuiInputTextFlags.EnterReturnsTrue)) {
                        if (renameData.updateName()) {
                            renameData = null;
                        }
                    }
                }
                ImGui.end();
            }
        }
        ImGui.end();
    }
}
