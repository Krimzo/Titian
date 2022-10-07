package entity.component;

import glparts.Mesh;
import glparts.Shaders;
import gui.abs.GUIRenderable;
import imgui.ImGui;
import renderer.abs.EditorRenderable;
import renderer.abs.GameRenderable;
import renderer.abs.IndexRenderable;

import java.io.Serializable;

public class MeshComponent implements GameRenderable, EditorRenderable, IndexRenderable, GUIRenderable, Serializable {
    public Mesh mesh = null;

    public MeshComponent() {}

    public MeshComponent(Mesh mesh) {
        this.mesh = mesh;
    }

    @Override
    public void gameRender(Shaders shaders) {
        if (mesh != null) {
            mesh.renderTriangles(shaders);
        }
    }

    @Override
    public void editorRender(Shaders shaders) {
        // In case of future changes
        if (mesh != null) {
            mesh.renderTriangles(shaders);
        }
    }

    @Override
    public void indexRender(Shaders shaders) {
        // In case of future changes
        if (mesh != null) {
            mesh.renderTriangles(shaders);
        }
    }

    @Override
    public void renderGUI() {
        ImGui.bulletText((mesh != null) ? mesh.getName() : "None");

        if (ImGui.beginPopupContextItem("EditEntityMesh")) {
            if (ImGui.button("Remove mesh")) {
                mesh = null;

                ImGui.closeCurrentPopup();
            }

            ImGui.endPopup();
        }
    }
}
