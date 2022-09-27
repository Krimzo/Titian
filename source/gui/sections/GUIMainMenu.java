package gui.sections;

import editor.Editor;
import gui.GUISection;
import imgui.ImGui;

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

            ImGui.endMenu();
        }
    }

    private void viewMenu() {
        if (ImGui.beginMenu("View")) {
            boolean wireframeState = (boolean) editor.savedData.get("WireframeState");
            wireframeState = ImGui.selectable("Wireframe State", wireframeState);
            editor.savedData.put("WireframeState", wireframeState);

            ImGui.endMenu();
        }
    }

    private void renderMenu() {
        if (ImGui.beginMenu("Render")) {
            if (ImGui.beginMenu("Index Texture")) {

                ImGui.endMenu();
            }

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
