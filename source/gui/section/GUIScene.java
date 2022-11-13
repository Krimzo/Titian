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
import utility.Instance;
import window.input.Input;

import java.util.concurrent.atomic.AtomicInteger;

public final class GUIScene extends GUISection {
    private GUITextInput textInput = null;

    public GUIScene(Editor editor) {
        super(editor);
    }

    private void addEntity() {
        if (ImGui.button("Entity", -1, 0)) {
            Entity entity = new Entity(editor.getScene().names.entity, "Entity", editor);
            editor.getScene().add(entity);

            // (Mesh, Material) bind after .add() call so defaults don't get added to the scene list
            entity.components.mesh.mesh = editor.data.defaultMesh;
            entity.components.material.material = editor.data.defaultMaterial;

            GUIPopup.close();
        }
    }

    private void addCamera() {
        if (ImGui.beginMenu("Camera")) {
            if (ImGui.button("Perspective", -1, 0)) {
                Entity camera = new PerspectiveCamera(editor.getScene().names.entity, "Perspective camera", editor);
                editor.getScene().add(camera);

                GUIPopup.close();
            }

            if (ImGui.button("Orthographic")) {
                Entity camera = new OrthographicCamera(editor.getScene().names.entity, "Orthographic camera", editor);
                editor.getScene().add(camera);

                GUIPopup.close();
            }

            ImGui.endMenu();
        }
    }

    private void addLight() {
        if (ImGui.beginMenu("Light")) {
            if (ImGui.button("Ambient", -1, 0)) {
                Entity light = new AmbientLight(editor.getScene().names.entity, "Ambient Light", editor);
                editor.getScene().add(light);

                GUIPopup.close();
            }

            if (ImGui.button("Directional")) {
                Entity camera = new DirectionalLight(editor.getScene().names.entity, "Directional Light", editor);
                editor.getScene().add(camera);

                GUIPopup.close();
            }

            if (ImGui.button("Positional", -1, 0)) {
                Entity camera = new PositionalLight(editor.getScene().names.entity, "Positional Light", editor);
                editor.getScene().add(camera);

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
        if (ImGui.begin("Scene", ImGuiWindowFlags.NoScrollbar)) {
            GUIPopup.windowPopup("SceneWindow", this::windowPopup);

            for (AtomicInteger i = new AtomicInteger(); i.get() < editor.getScene().size(); i.getAndIncrement()) {
                Entity entity = editor.getScene().get(i.get());

                if (ImGui.selectable(entity.getName(), entity == editor.getScene().selected.entity)) {
                    editor.getScene().selected.entity = entity;
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
                        Entity removed = editor.getScene().remove(i.getAndDecrement());

                        removed.eraseName();

                        if (removed == editor.getScene().selected.entity) {
                            editor.getScene().selected.entity = null;
                        }

                        if (removed == editor.getScene().selected.camera) {
                            editor.getScene().selected.camera = null;
                        }

                        GUIPopup.close();
                    }
                });
            }

            if (Instance.isValid(textInput)) {
                textInput.update();
            }
        }
        ImGui.end();
    }
}
