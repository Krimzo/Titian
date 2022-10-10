package entity.component;

import entity.Entity;
import entity.abs.EntityComponent;
import glparts.Mesh;
import glparts.Shaders;
import gui.helper.GUIPopup;
import imgui.ImGui;
import renderer.abs.Renderable;

import java.io.Serializable;

public class MeshComponent extends EntityComponent implements Renderable, Serializable {
    public Mesh mesh = null;

    public MeshComponent(Entity entity) {
        super(entity);
    }

    @Override
    public void dispose() {

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

        GUIPopup.itemPopup("EditEntityMesh", () -> {
            if (ImGui.button("Remove mesh")) {
                mesh = null;
                GUIPopup.close();
            }
        });
    }
}
