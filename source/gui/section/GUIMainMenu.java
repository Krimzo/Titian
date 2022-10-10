package gui.section;

import editor.Editor;
import entity.Entity;
import gui.GUIEdit;
import gui.GUIStyle;
import gui.abs.GUISection;
import imgui.ImGui;
import imgui.extension.imguifiledialog.ImGuiFileDialog;
import math.Float2;
import scene.Scene;

public final class GUIMainMenu extends GUISection {
    public GUIMainMenu(Editor editor) {
        super(editor);
    }

    private void fileMenu() {
        if (ImGui.beginMenu("File")) {
            if (ImGui.menuItem("Load scene")) {
                ImGuiFileDialog.openDialog("LoadSceneDlg", "Load scene", ".titian", ".", "", 1, 0, 0);
            }

            if (ImGui.menuItem("Save scene") && editor.scene != null) {
                ImGuiFileDialog.openDialog("SaveSceneDlg", "Save scene", ".titian", ".", "", 1, 0, 0);
            }

            if (ImGui.menuItem("Exit")) {
                editor.window.close();
            }

            ImGui.endMenu();
        }
    }

    private void editMenu() {
        if (ImGui.beginMenu("Edit")) {
            if (ImGui.menuItem("Reload scripts") && editor.scene != null) {
                for (Entity entity : editor.scene) {
                    entity.components.script.reload();
                }
            }

            ImGui.endMenu();
        }
    }

    private void viewMenu() {
        if (ImGui.beginMenu("View")) {
            if (ImGui.beginMenu("Colors")) {
                GUIEdit.editColor3("Black", GUIStyle.black);
                GUIEdit.editColor3("Dark", GUIStyle.dark);
                GUIEdit.editColor3("Normal", GUIStyle.normal);
                GUIEdit.editColor3("Light", GUIStyle.light);
                GUIEdit.editColor3("Special", GUIStyle.special);

                if (ImGui.button("Reload style", -1, 0)) {
                    GUIStyle.reloadStyle();
                }

                if (ImGui.button("Load defaults", -1, 0)) {
                    GUIStyle.loadDefaults();
                }

                ImGui.endMenu();
            }

            ImGui.endMenu();
        }
    }

    private void renderMenu() {
        if (ImGui.beginMenu("Render")) {
            Float2 size = new Float2(1280, 720);

            if (ImGui.beginMenu("Depth Texture")) {
                ImGui.image(editor.editorRenderer.renderBuffer.getDepthMap().getBuffer(), size.x, size.y, 0, 1, 1, 0);
                ImGui.endMenu();
            }

            if (ImGui.beginMenu("Index Texture")) {
                ImGui.image(editor.editorRenderer.indexBuffer.getColorMap().getBuffer(), size.x, size.y, 0, 1, 1, 0);
                ImGui.endMenu();
            }

            ImGui.endMenu();
        }
    }

    private void displayDialogs() {
        if (ImGuiFileDialog.display("LoadSceneDlg", 0, 192, 108, 1920, 1080)) {
            if (ImGuiFileDialog.isOk()) {
                editor.disposeCurrentScene();
                editor.scene = Scene.fromFile(ImGuiFileDialog.getFilePathName(), editor);
            }

            ImGuiFileDialog.close();
        }

        if (ImGuiFileDialog.display("SaveSceneDlg", 0, 192, 108, 1920, 1080)) {
            if (ImGuiFileDialog.isOk() && editor.scene != null) {
                editor.scene.toFile(ImGuiFileDialog.getFilePathName());
            }

            ImGuiFileDialog.close();
        }
    }

    @Override
    public void renderGUI() {
        if (ImGui.beginMainMenuBar()) {
            fileMenu();
            editMenu();
            viewMenu();
            renderMenu();

            displayDialogs();
        }
        ImGui.endMainMenuBar();
    }
}
