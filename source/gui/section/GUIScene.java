package gui.section;

import camera.OrthographicCamera;
import camera.PerspectiveCamera;
import editor.Editor;
import entity.Entity;
import gui.abs.GUISection;
import imgui.ImGui;
import imgui.flag.ImGuiInputTextFlags;
import imgui.flag.ImGuiWindowFlags;
import imgui.type.ImString;
import window.input.Input;

public final class GUIScene extends GUISection {
    private static class RenameData {
        public Entity entity;
        public ImString name;

        public RenameData(Entity entity) {
            this.entity = entity;
            name = new ImString(entity.getName(), 30);
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
                entity.components.mesh.mesh = editor.data.defaultMesh;
                entity.components.material.material = editor.data.defaultMaterial;
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

                if (ImGui.selectable(entity.getName(), entity == editor.scene.selected)) {
                    editor.scene.selected = entity;
                }

                if (!Input.isShiftDown() && ImGui.beginPopupContextItem("EditEntity" + i)) {
                    if (ImGui.button("Rename")) {
                        renameData = new RenameData(entity);
                    }

                    if (ImGui.button("Delete", -1, 0)) {
                        Entity removed = editor.scene.remove(i--);

                        if (removed == editor.scene.selected) {
                            editor.scene.selected = null;
                        }

                        if (removed == editor.scene.camera) {
                            editor.scene.camera = null;
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
