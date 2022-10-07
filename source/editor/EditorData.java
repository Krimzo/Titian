package editor;

import glparts.Mesh;
import glparts.Shaders;
import glparts.Texture;
import glparts.abs.Disposable;
import material.Material;

public class EditorData implements Disposable {
    public final Shaders frustumShaders;

    public final Mesh defaultMesh;
    public final Mesh frustumMesh;

    public final Texture defaultTexture;

    public final Material defaultMaterial;

    public boolean gameRunning = false;
    public boolean wireframeState = false;

    public EditorData(Editor editor) throws Exception {
        frustumShaders = new Shaders(editor.window.getContext(), "shaders/Frustum.glsl");

        defaultMesh = new Mesh(null, "Default Mesh", editor.window.getContext(), "resource/meshes/cube.obj");
        frustumMesh = Mesh.generateFrustumMesh();

        defaultTexture = new Texture(null, "Default Texture", editor.window.getContext(), "resource/textures/default.png");

        defaultMaterial = new Material(null, "Default Material");
        defaultMaterial.colorMap = defaultTexture;
        defaultMaterial.normalMap = defaultTexture;
        defaultMaterial.roughnessMap = defaultTexture;
    }

    @Override
    public void dispose() {
        defaultMesh.dispose();
        frustumMesh.dispose();

        defaultTexture.dispose();
    }
}
