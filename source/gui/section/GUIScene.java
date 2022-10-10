package gui.section;

import camera.OrthographicCamera;
import camera.PerspectiveCamera;
import editor.Editor;
import entity.Entity;
import gui.GUIPopup;
import gui.GUITextInput;
import gui.abs.GUISection;
import imgui.ImGui;
import imgui.flag.ImGuiWindowFlags;
import window.input.Input;

import java.util.concurrent.atomic.AtomicInteger;

public final class GUIScene extends GUISection {
    private GUITextInput textInput = null;

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

                GUIPopup.close();
            }

            if (ImGui.beginMenu("Camera")) {
                if (ImGui.button("Perspective", -1, 0)) {
                    PerspectiveCamera camera = new PerspectiveCamera(editor.scene.entityNames, "Perspective camera", editor);
                    editor.scene.add(camera);

                    GUIPopup.close();
                }

                if (ImGui.button("Orthographic")) {
                    OrthographicCamera camera = new OrthographicCamera(editor.scene.entityNames, "Orthographic camera", editor);
                    editor.scene.add(camera);

                    GUIPopup.close();
                }

                ImGui.endMenu();
            }

            ImGui.endMenu();
        }
    }

    @Override
    public void renderGUI() {
        if (ImGui.begin("Scene", ImGuiWindowFlags.NoScrollbar) && editor.scene != null) {
            GUIPopup.windowPopup("SceneWindow", this::windowPopup);

            for (AtomicInteger i = new AtomicInteger(); i.get() < editor.scene.size(); i.getAndIncrement()) {
                Entity entity = editor.scene.get(i.get());

                if (ImGui.selectable(entity.getName(), entity == editor.scene.selected)) {
                    editor.scene.selected = entity;
                }

                GUIPopup.itemPopup("EditEntity" + i, () -> {
                    if (Input.isShiftDown()) {
                        return;
                    }

                    if (ImGui.button("Rename")) {
                        textInput = new GUITextInput(entity.getName(), name -> {
                            entity.setName(name);
                            textInput = null;
                        });
                    }

                    if (ImGui.button("Delete", -1, 0)) {
                        Entity removed = editor.scene.remove(i.getAndDecrement());

                        if (removed == editor.scene.selected) {
                            editor.scene.selected = null;
                        }

                        if (removed == editor.scene.camera) {
                            editor.scene.camera = null;
                        }

                        GUIPopup.close();
                    }
                });
            }

            if (textInput != null) {
                textInput.update();
            }
        }
        ImGui.end();
    }
}
