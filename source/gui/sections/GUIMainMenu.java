package gui.sections;

import editor.Editor;
import gui.GUIRenderable;
import gui.GUISection;
import imgui.ImGui;

public class GUIMainMenu extends GUISection implements GUIRenderable {
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

            ImGui.endMenu();
        }
    }

    private void viewMenu() {
        if (ImGui.beginMenu("View")) {

            ImGui.endMenu();
        }
    }

    private void renderMenu() {
        if (ImGui.beginMenu("Render")) {

            ImGui.endMenu();
        }
    }

    @Override
    public void onGUIRender() {
        if (ImGui.beginMainMenuBar()) {
            fileMenu();
            editMenu();
            viewMenu();
            renderMenu();
        }
        ImGui.endMainMenuBar();
    }
}
