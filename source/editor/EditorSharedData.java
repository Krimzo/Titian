package editor;

import glparts.Mesh;
import glparts.Shaders;
import glparts.Texture;
import glparts.abs.Disposable;
import imgui.extension.imguizmo.flag.Mode;
import material.Material;
import math.Int2;

public class EditorSharedData implements Disposable {
    public final Shaders frustumShaders;
    public final Shaders unlitShaders;

    public final Mesh defaultMesh;
    public final Mesh frustumMesh;

    public final Texture defaultTexture;

    public final Material defaultMaterial;

    public boolean gameRunning = false;
    public boolean wireframeState = false;
    public boolean renderGrid = true;

    public int gizmoOperation = 0;
    public int gizmoMode = Mode.WORLD;

    public EditorSharedData(Editor editor) throws Exception {
        frustumShaders = new Shaders(editor.window.getContext(), "shaders/Frustum.glsl");
        unlitShaders = new Shaders(editor.window.getContext(), "shaders/Unlit.glsl");

        defaultMesh = new Mesh(null, "Default Mesh", editor.window.getContext(), "resource/meshes/cube.obj");
        frustumMesh = Mesh.generateFrustumMesh();

        defaultTexture = new Texture(null, "Default Texture", editor.window.getContext(),
            new Int2(1, 1), new byte[] { (byte) 50, (byte) 50, (byte) 50, (byte) 0xFF }
        );

        defaultMaterial = new Material(null, "Default Material");
        defaultMaterial.colorMap = defaultTexture;
        defaultMaterial.normalMap = defaultTexture;
        defaultMaterial.roughnessMap = defaultTexture;
    }

    @Override
    public void dispose() {
        frustumShaders.dispose();
        unlitShaders.dispose();

        defaultMesh.dispose();
        frustumMesh.dispose();

        defaultTexture.dispose();
    }
}
