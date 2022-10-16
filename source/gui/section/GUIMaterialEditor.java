package gui.section;

import camera.PerspectiveCamera;
import editor.Editor;
import entity.Entity;
import glparts.Texture;
import gui.abs.GUISection;
import gui.helper.*;
import imgui.ImGui;
import imgui.ImVec2;
import light.AmbientLight;
import light.DirectionalLight;
import material.Material;
import math.Float2;
import math.Float3;
import math.Int2;
import scene.Scene;
import utility.Files;
import utility.Instance;
import window.input.Input;
import window.input.Mouse;

import java.util.Iterator;

public final class GUIMaterialEditor extends GUISection {
    private GUITextInput textInput = null;

    private Object selected = null;

    private final PerspectiveCamera camera;
    private final Float2 startCameraInfo;
    private final Float2 cameraInfo;

    private final Entity entity;
    private final Scene scene;

    public GUIMaterialEditor(Editor editor) {
        super(editor);

        camera = new PerspectiveCamera(null, null, editor);
        startCameraInfo = new Float2();
        cameraInfo = new Float2();
        camera.components.transform.position.z = 1;
        camera.sensitivity = 0.5f;

        entity = new Entity(null, null, editor);
        entity.components.mesh.mesh = editor.data.defaultMesh;

        scene = new Scene();
        scene.selected.ambientLight = new AmbientLight(null, null, editor);
        scene.selected.directionalLight = new DirectionalLight(null, null, editor);
        scene.add(entity);
    }

    private void displayMaterials(Scene scene) {
        int counter = 0;
        Iterator<Material> it = scene.materials.iterator();

        while (it.hasNext()) {
            final Material material = it.next();

            final boolean headerState = ImGui.collapsingHeader(material.getName());

            GUIPopup.itemPopup("EditMaterials" + counter++, () -> {
                if (ImGui.button("Display", -1, 0)) {
                    selected = material;
                    GUIPopup.close();
                }

                if (ImGui.button("Rename")) {
                    textInput = new GUITextInput(material.getName(), name -> {
                        material.setName(name);
                        textInput = null;
                    });
                }

                if (ImGui.button("Delete", -1, 0)) {
                    it.remove();

                    material.eraseName();

                    if (material == selected) {
                        selected = null;
                    }

                    for (Entity entity : scene) {
                        if (entity.components.material.material == material) {
                            entity.components.material.material = null;
                        }
                    }

                    GUIPopup.close();
                }
            });

            if (headerState) {
                final String id = "##" + material.getName();

                material.colorBlend = GUIEdit.editFloat("Blend" + id, material.colorBlend, 0.01f, 0, 1);

                GUIEdit.editColor3("Color" + id, material.color);

                if (ImGui.selectable(Instance.isValid(material.colorMap) ? material.colorMap.getName() : "None")) {
                    selected = material.colorMap;
                }
                GUIDragDrop.getData("TextureTransfer", texture -> {
                    material.colorMap = (Texture) texture;
                });
                GUIPopup.itemPopup("EditMaterialColorMap" + counter++, () -> {
                    if (ImGui.button("Remove")) {
                        material.colorMap = null;
                        GUIPopup.close();
                    }
                });

                material.roughness = GUIEdit.editFloat("Roughness" + id, material.roughness, 0.01f);

                if (ImGui.selectable(Instance.isValid(material.roughnessMap) ? material.roughnessMap.getName() : "None")) {
                    selected = material.roughnessMap;
                }
                GUIDragDrop.getData("TextureTransfer", texture -> {
                    material.roughnessMap = (Texture) texture;
                });
                GUIPopup.itemPopup("EditMaterialRoughnessMap" + counter++, () -> {
                    if (ImGui.button("Remove")) {
                        material.roughnessMap = null;
                        GUIPopup.close();
                    }
                });

                if (ImGui.selectable(Instance.isValid(material.normalMap) ? material.normalMap.getName() : "None")) {
                    selected = material.normalMap;
                }
                GUIDragDrop.getData("TextureTransfer", texture -> {
                    material.normalMap = (Texture) texture;
                });
                GUIPopup.itemPopup("EditMaterialNormalMap" + counter++, () -> {
                    if (ImGui.button("Remove")) {
                        material.normalMap = null;
                        GUIPopup.close();
                    }
                });
            }
        }

        GUIPopup.windowPopup("NewMaterial", () -> {
            if (ImGui.button("New")) {
                textInput = new GUITextInput("", name -> {
                    scene.materials.add(new Material(scene.names.material, name));
                    textInput = null;
                });
            }
        });
    }

    private void displayTextures(Scene scene) {
        int counter = 0;
        Iterator<Texture> it = scene.textures.iterator();

        while (it.hasNext()) {
            Texture texture = it.next();

            if (ImGui.selectable(texture.getName(), texture == selected)) {
                selected = texture;
            }

            GUIDragDrop.setData("TextureTransfer", texture);

            GUIPopup.itemPopup("EditTextures" + counter++, () -> {
                if (ImGui.button("Rename")) {
                    textInput = new GUITextInput(texture.getName(), name -> {
                        texture.setName(name);
                        textInput = null;
                    });
                }

                if (ImGui.button("Delete", -1, 0)) {
                    it.remove();

                    texture.eraseName();

                    if (texture == selected) {
                        selected = null;
                    }

                    for (Entity entity : scene) {
                        if (Instance.isValid(entity.components.material.material)) {
                            if (entity.components.material.material.colorMap == texture) {
                                entity.components.material.material.colorMap = null;
                            }
                            if (entity.components.material.material.normalMap == texture) {
                                entity.components.material.material.normalMap = null;
                            }
                            if (entity.components.material.material.roughnessMap == texture) {
                                entity.components.material.material.roughnessMap = null;
                            }
                        }
                    }

                    GUIPopup.close();
                }
            });
        }
    }

    private void updateCamera() {
        Float3 position = camera.components.transform.position;

        if (Input.isMousePressed(Mouse.Right)) {
            startCameraInfo.set(cameraInfo);
        }

        if (Input.isMouseDown(Mouse.Right)) {
            ImVec2 dragDelta = ImGui.getMouseDragDelta(Mouse.Right);
            cameraInfo.x = startCameraInfo.x + dragDelta.x * camera.sensitivity;
            cameraInfo.y = startCameraInfo.y + dragDelta.y * camera.sensitivity;
            cameraInfo.y = Math.min(Math.max(cameraInfo.y, -85), 85);

            position.x = (float) -Math.sin(Math.toRadians(cameraInfo.x));
            position.z = (float) Math.cos(Math.toRadians(cameraInfo.x));
            position.y = (float) Math.tan(Math.toRadians(cameraInfo.y));
        }

        float cameraDistance = entity.components.mesh.mesh.maxRadius + 1;
        position.set(position.normalize().multiply(cameraDistance));
        camera.setForward(position.negate());

        scene.selected.directionalLight.setDirection(camera.getForward());
    }

    private void displayMaterial(Int2 viewportSize) {
        editor.editorRenderer.clear(camera);

        editor.window.getContext().setViewport(viewportSize);
        editor.editorRenderer.resize(viewportSize);
        camera.updateAspect(viewportSize);

        editor.window.getContext().setWireframe(editor.data.wireframeState);
        editor.editorRenderer.renderScene(scene, camera);
        editor.window.getContext().setWireframe(false);

        GUIDisplay.texture(editor.editorRenderer.renderBuffer.getColorMap(), viewportSize);
    }

    @Override
    public void renderGUI() {
        if (ImGui.begin("Material Editor")) {
            final float availWidth = ImGui.getContentRegionAvailX();

            ImGui.columns(3, "MaterialEditorColumns", false);
            ImGui.setColumnWidth(ImGui.getColumnIndex(), availWidth * 0.25f);
            if (ImGui.beginChild("Materials")) {
                displayMaterials(editor.getScene());
            }
            ImGui.endChild();

            ImGui.nextColumn();
            ImGui.setColumnWidth(ImGui.getColumnIndex(), availWidth * 0.25f);
            if (ImGui.beginChild("Textures")) {
                displayTextures(editor.getScene());
            }
            ImGui.endChild();

            GUIDragDrop.getData("ImageFile", path -> {
                try {
                    String fileName = Files.getNameWithoutExtension((String) path);
                    editor.getScene().textures.add(new Texture(editor.getScene().names.texture, fileName, editor.window.getContext(), (String) path));
                }
                catch (Exception ignored) {
                    System.out.println("Texture \"" + path + "\" loading error!");
                }
            });

            ImGui.nextColumn();
            if (ImGui.beginChild("Texture View")) {
                Int2 viewportSize = new Int2((int) ImGui.getContentRegionAvailX(), (int) ImGui.getContentRegionAvailY());

                if (Instance.isValid(selected)) {
                    if (selected.getClass() == Material.class) {
                        entity.components.material.material = (Material) selected;
                        updateCamera();
                        displayMaterial(viewportSize);
                    }
                    else if (selected.getClass() == Texture.class) {
                        GUIDisplay.texture((Texture) selected, viewportSize);
                    }
                }
            }
            ImGui.endChild();

            if (Instance.isValid(textInput)) {
                textInput.update();
            }
        }
        ImGui.end();
    }
}