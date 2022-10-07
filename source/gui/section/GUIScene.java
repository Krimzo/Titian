package gui.section;

import camera.OrthographicCamera;
import camera.PerspectiveCamera;
import editor.Editor;
import entity.Entity;
import gui.abs.GUISection;
import imgui.ImGui;
import imgui.flag.ImGuiWindowFlags;

public final class GUIScene extends GUISection {
    public GUIScene(Editor editor) {
        super(editor);
    }

    @Override
    public void renderGUI() {
        if (ImGui.begin("Scene", ImGuiWindowFlags.NoScrollbar)) {
            if (editor.scene != null) {
                if (ImGui.beginPopupContextWindow()) {
                    if (ImGui.beginMenu("New")) {
                        if (ImGui.button("Entity")) {
                            Entity entity = new Entity(editor.scene.entityNames, "NewEntity");
                            entity.meshComponent.mesh = editor.data.defaultMesh;
                            entity.materialComponent.material = editor.data.defaultMaterial;
                            editor.scene.add(entity);

                            ImGui.closeCurrentPopup();
                        }

                        if (ImGui.beginMenu("Camera")) {
                            if (ImGui.button("Perspective")) {
                                PerspectiveCamera camera = new PerspectiveCamera(editor.scene.entityNames, "NewPerspCam");
                                editor.scene.add(camera);

                                ImGui.closeCurrentPopup();
                            }

                            if (ImGui.button("Orthographic")) {
                                OrthographicCamera camera = new OrthographicCamera(editor.scene.entityNames, "NewOrthoCam");
                                editor.scene.add(camera);

                                ImGui.closeCurrentPopup();
                            }

                            ImGui.endMenu();
                        }

                        ImGui.endMenu();
                    }

                    ImGui.endPopup();
                }

                for (int i = 0; i < editor.scene.size(); i++) {
                    Entity entity = editor.scene.get(i);

                    if (ImGui.selectable(entity.getName(), entity == editor.scene.selectedEntity)) {
                        editor.scene.selectedEntity = entity;
                    }

                    if (!editor.window.keyboard.shift.getState() && ImGui.beginPopupContextItem("EditEntity" + i)) {
                        if (ImGui.button("Delete")) {
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
            }
        }
        ImGui.end();
    }
}
