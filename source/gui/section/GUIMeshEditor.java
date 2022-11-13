package gui.section;

import camera.PerspectiveCamera;
import editor.Editor;
import entity.Entity;
import entity.component.MeshComponent;
import entity.material.Material;
import glparts.Mesh;
import glparts.Texture;
import gui.abs.GUISection;
import gui.helper.*;
import imgui.ImGui;
import imgui.ImVec2;
import imgui.flag.ImGuiWindowFlags;
import light.AmbientLight;
import light.DirectionalLight;
import math.Float2;
import math.Float3;
import math.Int2;
import scene.Scene;
import utility.Instance;
import utility.helper.FileHelper;
import window.input.Input;
import window.input.Mouse;

import java.util.Iterator;

public final class GUIMeshEditor extends GUISection {
    private GUITextInput textInput = null;

    private final PerspectiveCamera camera;
    private final Float2 startCameraInfo;
    private final Float2 cameraInfo;

    private final Entity entity;
    private final Scene scene;

    public GUIMeshEditor(Editor editor) {
        super(editor);

        camera = new PerspectiveCamera(null, null, editor);
        startCameraInfo = new Float2();
        cameraInfo = new Float2();
        camera.components.transform.position.z = 1;
        camera.sensitivity = 0.5f;

        Material material = new Material(null, "Mesh Editor Material");
        material.colorMap = new Texture(null, "Mesh Editor Texture", editor.window.getContext(),
            new Int2(1, 1), new byte[] { (byte) 200, (byte) 200, (byte) 200, (byte) 0xFF }
        );

        entity = new Entity(null, null, editor);
        entity.components.material.material = material;

        scene = new Scene();
        scene.selected.ambientLight = new AmbientLight(null, null, editor);
        scene.selected.directionalLight = new DirectionalLight(null, null, editor);
        scene.add(entity);
    }

    private void displayMeshes(Scene scene) {
        int counter = 0;
        Iterator<Mesh> it = scene.meshes.iterator();

        final MeshComponent mc = entity.components.mesh;

        while (it.hasNext()) {
            Mesh mesh = it.next();

            if (ImGui.selectable(mesh.getName(), mesh == mc.mesh)) {
                mc.mesh = mesh;
            }

            GUIPopup.itemPopup("EditMeshes" + counter++, () -> {
                if (ImGui.button("Rename")) {
                    textInput = new GUITextInput(mesh.getName(), name -> {
                        mesh.setName(name);
                        textInput = null;
                    });
                }

                if (ImGui.button("Delete", -1, 0)) {
                    it.remove();

                    mesh.eraseName();

                    if (mesh == mc.mesh) {
                        mc.mesh = null;
                    }

                    for (Entity entity : scene) {
                        if (entity.components.mesh.mesh == mesh) {
                            entity.components.mesh.mesh = null;
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

    private void renderSelectedMesh(Int2 viewportSize) {
        editor.editorRenderer.clear(camera);

        editor.window.getContext().setViewport(viewportSize);
        editor.editorRenderer.resize(viewportSize);
        camera.updateAspect(viewportSize);

        editor.window.getContext().setWireframe(editor.data.wireframeState);
        editor.editorRenderer.renderScene(scene, camera);
        editor.window.getContext().setWireframe(false);
    }

    @Override
    public void renderGUI() {
        if (ImGui.begin("Mesh Editor", ImGuiWindowFlags.NoScrollbar)) {
            final float availWidth = ImGui.getContentRegionAvailX();

            ImGui.columns(2, "MeshEditorColumns", false);
            ImGui.setColumnWidth(ImGui.getColumnIndex(), availWidth * 0.25f);
            if (ImGui.beginChild("Meshes")) {
                displayMeshes(editor.getScene());
            }
            ImGui.endChild();

            GUIDragDrop.getData("MeshFile", path -> {
                try {
                    String fileName = FileHelper.getNameWithoutExtension((String) path);
                    editor.getScene().meshes.add(new Mesh(editor.getScene().names.mesh, fileName, editor.window.getContext(), (String) path));
                }
                catch (Exception ignored) {
                    System.out.println("Mesh \"" + path + "\" loading error!");
                }
            });

            ImGui.nextColumn();
            if (ImGui.beginChild("Mesh View") && Instance.isValid(entity.components.mesh.mesh)) {
                Int2 viewportSize = new Int2((int) ImGui.getContentRegionAvailX(), (int) ImGui.getContentRegionAvailY());
                updateCamera();
                renderSelectedMesh(viewportSize);
                GUIDisplay.texture(editor.editorRenderer.renderBuffer.getColorMap(), viewportSize);
            }
            ImGui.endChild();

            if (ImGui.begin("Mesh")) {
                Mesh mesh = entity.components.mesh.mesh;
                if (Instance.isValid(mesh)) {
                    ImGui.bulletText(mesh.getName());
                    GUIEdit.editInt("Vertex count", mesh.getVertexCount(), 0);
                    GUIEdit.editFloat("Max radius", mesh.maxRadius, 0);
                }
            }
            ImGui.end();

            if (Instance.isValid(textInput)) {
                textInput.update();
            }
        }
        ImGui.end();
    }
}
