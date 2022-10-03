package gui.section;

import editor.Editor;
import gui.abs.GUISection;
import imgui.ImGui;
import math.Int2;

public final class GUIMainMenu extends GUISection {
    public GUIMainMenu(Editor editor) {
        super(editor);
    }

    private void fileMenu() {
        if (ImGui.beginMenu("File")) {
            if (ImGui.menuItem("Exit")) {
                editor.window.close();
            }

            ImGui.endMenu();
        }
    }

    private void editMenu() {
        if (ImGui.beginMenu("Edit")) {
            ImGui.menuItem("WIP");
            ImGui.endMenu();
        }
    }

    private void viewMenu() {
        if (ImGui.beginMenu("View")) {
            ImGui.menuItem("WIP");
            ImGui.endMenu();
        }
    }

    private void renderMenu() {
        if (ImGui.beginMenu("Render")) {
            Int2 viewportSize = (Int2) editor.savedData.get("ViewportSize");

            if (ImGui.beginMenu("Depth Texture")) {
                ImGui.image(editor.renderer.renderBuffer.getDepthMap().getBuffer(), viewportSize.x, viewportSize.y, 0, 1, 1, 0);
                ImGui.endMenu();
            }

            if (ImGui.beginMenu("Index Texture")) {
                ImGui.image(editor.renderer.indexBuffer.getColorMap().getBuffer(), viewportSize.x, viewportSize.y, 0, 1, 1, 0);
                ImGui.endMenu();
            }

            ImGui.endMenu();
        }
    }

    @Override
    public void renderGUI() {
        if (ImGui.beginMainMenuBar()) {
            fileMenu();
            editMenu();
            viewMenu();
            renderMenu();
        }
        ImGui.endMainMenuBar();
    }
}
