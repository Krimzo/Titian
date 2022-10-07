package editor;

import glparts.Mesh;
import glparts.Texture;
import material.Material;

public class EditorData {
    public final Mesh defaultMesh;
    public final Material defaultMaterial;

    public boolean gameRunning = false;
    public boolean wireframeState = false;

    public EditorData(Editor editor) throws Exception {
        defaultMesh = new Mesh(null, "Default Mesh", editor.window.getContext(), "resource/meshes/cube.obj");
        defaultMaterial = new Material(null, "Default Material");
        defaultMaterial.colorMap = new Texture(null, "Default Texture", editor.window.getContext(), "resource/textures/default.png");
    }
}
