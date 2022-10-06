package entity.component;

import glparts.Mesh;
import glparts.Shaders;
import gui.abs.GUIRenderable;
import imgui.ImGui;
import renderer.abs.Renderable;

import java.io.Serializable;

public class MeshComponent implements Renderable, GUIRenderable, Serializable {
    public Mesh mesh = null;

    public MeshComponent() {}

    public MeshComponent(Mesh mesh) {
        this.mesh = mesh;
    }

    @Override
    public void render(Shaders shaders) {
        if (mesh != null) {
            mesh.render(shaders);
        }
    }

    @Override
    public void renderGUI() {
        ImGui.text((mesh != null) ? mesh.getName() : "NULL");

        if (ImGui.beginPopupContextItem("EditEntityMesh")) {
            if (ImGui.button("Remove mesh")) {
                mesh = null;

                ImGui.closeCurrentPopup();
            }

            ImGui.endPopup();
        }
    }
}
