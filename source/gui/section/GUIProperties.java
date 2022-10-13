package gui.section;

import editor.Editor;
import entity.Entity;
import glparts.Mesh;
import gui.abs.GUISection;
import gui.helper.GUIDragDrop;
import gui.helper.GUIPopup;
import imgui.ImGui;
import imgui.ImVec2;
import imgui.flag.ImGuiTreeNodeFlags;
import material.Material;
import script.Script;

public final class GUIProperties extends GUISection {
    public GUIProperties(Editor editor) {
        super(editor);
    }

    @Override
    public void dispose() {

    }

    private void renderTransform(Entity selected) {
        selected.components.transform.renderGUI();
    }

    private void renderMesh(Entity selected) {
        selected.components.mesh.renderGUI();

        if (ImGui.beginListBox("##Loaded meshes", -1, 0)) {
            for (Mesh mesh : editor.scene.meshes) {
                if (ImGui.selectable(mesh.getName(), selected.components.mesh.mesh == mesh)) {
                    selected.components.mesh.mesh = mesh;
                }
            }

            ImGui.endListBox();
        }
    }

    private void renderMaterial(Entity selected) {
        selected.components.material.renderGUI();

        if (ImGui.beginListBox("##Loaded materials", -1, 0)) {
            for (Material material : editor.scene.materials) {
                if (ImGui.selectable(material.getName(), selected.components.material.material == material)) {
                    selected.components.material.material = material;
                }
            }

            ImGui.endListBox();
        }
    }

    private void renderPhysics(Entity selected) {
        selected.components.physics.renderGUI();
    }

    private void renderScripts(Entity selected) {
        selected.components.script.renderGUI();

        ImVec2 contentMin = ImGui.getWindowContentRegionMin();
        ImVec2 contentMax = ImGui.getWindowContentRegionMax();
        ImGui.dummy(contentMax.x - contentMin.x, contentMax.y - contentMin.y);

        GUIDragDrop.getData("ScriptFile", path -> {
            selected.components.script.scripts.add(new Script((String) path, selected));
        });

        GUIPopup.itemPopup("Scripts", () -> {
            if (ImGui.button("Reload")) {
                selected.components.script.reload();
                GUIPopup.close();
            }
        });
    }

    @Override
    public void renderGUI() {
        Entity selected = (editor.scene != null) ? editor.scene.selected.entity : null;

        if (ImGui.begin("Properties") && selected != null) {
            if (ImGui.collapsingHeader("Info", ImGuiTreeNodeFlags.DefaultOpen)) {
                selected.renderInfoGUI(editor);
                ImGui.separator();
            }

            if (ImGui.collapsingHeader("Transform", ImGuiTreeNodeFlags.DefaultOpen)) {
                renderTransform(selected);
                ImGui.separator();
            }

            if (ImGui.collapsingHeader("Mesh", ImGuiTreeNodeFlags.DefaultOpen)) {
                renderMesh(selected);
                ImGui.separator();
            }

            if (ImGui.collapsingHeader("Material", ImGuiTreeNodeFlags.DefaultOpen)) {
                renderMaterial(selected);
                ImGui.separator();
            }

            if (ImGui.collapsingHeader("Physics", ImGuiTreeNodeFlags.DefaultOpen)) {
                renderPhysics(selected);
                ImGui.separator();
            }

            if (ImGui.collapsingHeader("Scripts", ImGuiTreeNodeFlags.DefaultOpen)) {
                renderScripts(selected);
            }
        }
        ImGui.end();
    }
}
