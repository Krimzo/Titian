package gui.section;

import camera.OrthographicCamera;
import camera.PerspectiveCamera;
import editor.Editor;
import entity.Entity;
import gui.abs.GUISection;
import gui.helper.GUIPopup;
import gui.helper.GUITextInput;
import imgui.ImGui;
import imgui.flag.ImGuiWindowFlags;
import light.AmbientLight;
import light.DirectionalLight;
import light.PositionalLight;
import window.input.Input;

import java.util.concurrent.atomic.AtomicInteger;

public final class GUIScene extends GUISection {
    private GUITextInput textInput = null;

    public GUIScene(Editor editor) {
        super(editor);
    }

    @Override
    public void dispose() {

    }

    private void addEntity() {
        if (ImGui.button("Entity", -1, 0)) {
            Entity entity = new Entity(editor.scene.names.entity, "Entity", editor);
            entity.components.mesh.mesh = editor.data.defaultMesh;
            entity.components.material.material = editor.data.defaultMaterial;
            editor.scene.addUnsaved(entity);

            GUIPopup.close();
        }
    }

    private void addCamera() {
        if (ImGui.beginMenu("Camera")) {
            if (ImGui.button("Perspective", -1, 0)) {
                Entity camera = new PerspectiveCamera(editor.scene.names.entity, "Perspective camera", editor);
                editor.scene.add(camera);

                GUIPopup.close();
            }

            if (ImGui.button("Orthographic")) {
                Entity camera = new OrthographicCamera(editor.scene.names.entity, "Orthographic camera", editor);
                editor.scene.add(camera);

                GUIPopup.close();
            }

            ImGui.endMenu();
        }
    }

    private void addLight() {
        if (ImGui.beginMenu("Light")) {
            if (ImGui.button("Ambient", -1, 0)) {
                Entity light = new AmbientLight(editor.scene.names.entity, "Ambient Light", editor);
                editor.scene.add(light);

                GUIPopup.close();
            }

            if (ImGui.button("Directional")) {
                Entity camera = new DirectionalLight(editor.scene.names.entity, "Directional Light", editor);
                editor.scene.add(camera);

                GUIPopup.close();
            }

            if (ImGui.button("Positional", -1, 0)) {
                Entity camera = new PositionalLight(editor.scene.names.entity, "Positional Light", editor);
                editor.scene.add(camera);

                GUIPopup.close();
            }

            ImGui.endMenu();
        }
    }

    private void windowPopup() {
        if (ImGui.beginMenu("New")) {
            addEntity();
            addCamera();
            addLight();

            ImGui.endMenu();
        }
    }

    @Override
    public void renderGUI() {
        if (ImGui.begin("Scene", ImGuiWindowFlags.NoScrollbar) && editor.scene != null) {
            GUIPopup.windowPopup("SceneWindow", this::windowPopup);

            for (AtomicInteger i = new AtomicInteger(); i.get() < editor.scene.size(); i.getAndIncrement()) {
                Entity entity = editor.scene.get(i.get());

                if (ImGui.selectable(entity.getName(), entity == editor.scene.selected.entity)) {
                    editor.scene.selected.entity = entity;
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

                        if (removed == editor.scene.selected.entity) {
                            editor.scene.selected.entity = null;
                        }

                        if (removed == editor.scene.selected.camera) {
                            editor.scene.selected.camera = null;
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
