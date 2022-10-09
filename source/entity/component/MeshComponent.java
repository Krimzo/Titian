package entity.component;

import entity.Entity;
import entity.abs.EntityComponent;
import glparts.Mesh;
import glparts.Shaders;
import imgui.ImGui;
import renderer.abs.EditorRenderable;
import renderer.abs.GameRenderable;
import renderer.abs.IndexRenderable;

import java.io.Serializable;

public class MeshComponent extends EntityComponent implements GameRenderable, EditorRenderable, IndexRenderable, Serializable {
    public Mesh mesh = null;

    public MeshComponent(Entity entity) {
        super(entity);
    }

    public MeshComponent(Entity entity, Mesh mesh) {
        super(entity);
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
