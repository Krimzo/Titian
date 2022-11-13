package editor;

import entity.material.Material;
import glparts.Mesh;
import glparts.Shaders;
import imgui.extension.imguizmo.flag.Mode;

public class EditorData {
    public final Shaders frustumShaders;
    public final Shaders unlitShaders;

    public final Mesh defaultMesh;
    public final Mesh frustumMesh;

    public final Material defaultMaterial;

    public boolean gameRunning = false;
    public boolean wireframeState = false;
    public boolean renderGrid = true;

    public int gizmoOperation = 0;
    public int gizmoMode = Mode.WORLD;

    public EditorData(Editor editor) throws Exception {
        frustumShaders = new Shaders(editor.window.getContext(), "shaders/Frustum.glsl");
        unlitShaders = new Shaders(editor.window.getContext(), "shaders/Unlit.glsl");

        defaultMesh = new Mesh(null, "Default Mesh", editor.window.getContext(), "resource/meshes/cube.obj");
        frustumMesh = Mesh.generateFrustumMesh();

        defaultMaterial = new Material(null, "Default Material");
    }
}
