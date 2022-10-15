package gui.section;

import editor.Editor;
import entity.Entity;
import glparts.Texture;
import gui.abs.GUISection;
import gui.helper.GUIDragDrop;
import gui.helper.GUIEdit;
import gui.helper.GUIPopup;
import gui.helper.GUITextInput;
import imgui.ImGui;
import imgui.flag.ImGuiTreeNodeFlags;
import material.Material;
import math.Int2;
import scene.Scene;
import utility.Files;
import utility.Instance;

import java.util.Iterator;

public final class GUIMaterialEditor extends GUISection {
    private GUITextInput textInput = null;

    private Object selected = null;

    public GUIMaterialEditor(Editor editor) {
        super(editor);
    }

    private void displayMaterials(Scene scene) {
        int counter = 0;
        Iterator<Material> it = scene.materials.iterator();

        while (it.hasNext()) {
            final Material material = it.next();

            if (ImGui.collapsingHeader(material.getName())) {
                final String id = "##" + material.getName();

                GUIEdit.editColor3("Color" + id, material.color);
                material.roughness = GUIEdit.editFloat("Roughness" + id, material.roughness, 0.01f);

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
            }

            GUIPopup.itemPopup("EditMaterials" + counter++, () -> {
                if (ImGui.button("Rename")) {
                    textInput = new GUITextInput(material.getName(), name -> {
                        material.setName(name);
                        textInput = null;
                    });
                }

                if (ImGui.button("Delete", -1, 0)) {
                    it.remove();

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

    private void displayMaterial(Material material, Int2 viewportSize) {

    }

    private void displayTexture(Texture texture, Int2 viewportSize) {
        ImGui.image(texture.getBuffer(),
            viewportSize.x, viewportSize.y,
            0, 1,
            1, 0
        );
    }

    @Override
    public void renderGUI() {
        if (ImGui.begin("Material Editor")) {
            ImGui.columns(3);
            if (ImGui.beginChild("Materials") && Instance.isValid(editor.scene)) {
                displayMaterials(editor.scene);
            }
            ImGui.endChild();

            ImGui.nextColumn();
            if (ImGui.beginChild("Textures") && Instance.isValid(editor.scene)) {
                displayTextures(editor.scene);
            }
            ImGui.endChild();

            GUIDragDrop.getData("ImageFile", path -> {
                if (Instance.isValid(editor.scene)) {
                    try {
                        String fileName = Files.getNameWithoutExtension((String) path);
                        editor.scene.textures.add(new Texture(editor.scene.names.texture, fileName, editor.window.getContext(), (String) path));
                    }
                    catch (Exception ignored) {
                        System.out.println("Texture \"" + path + "\" loading error!");
                    }
                }
            });

            ImGui.nextColumn();
            if (ImGui.beginChild("Texture View")) {
                if (Instance.isValid(selected)) {
                    Int2 viewportSize = new Int2((int) ImGui.getContentRegionAvailX(), (int) ImGui.getContentRegionAvailY());
                    if (selected.getClass() == Material.class) {
                        displayMaterial((Material) selected, viewportSize);
                    }
                    else {
                        displayTexture((Texture) selected, viewportSize);
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