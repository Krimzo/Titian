package gui.section;

import editor.Editor;
import entity.Entity;
import gui.abs.GUISection;
import gui.helper.GUIDisplay;
import gui.helper.GUIEdit;
import gui.helper.GUIStyle;
import gui.helper.GUITextInput;
import imgui.ImGui;
import imgui.ImVec2;
import imgui.ImVec4;
import imgui.flag.ImGuiStyleVar;
import math.Float2;
import math.Int2;
import utility.Instance;
import utility.helper.FileHelper;
import window.input.Input;
import window.input.Mouse;

public final class GUIMainMenu extends GUISection {
    private GUITextInput textInput = null;

    public GUIMainMenu(Editor editor) {
        super(editor);
    }

    private void windowControl() {
        ImVec4 rect = new ImVec4(ImGui.getWindowPosX(), ImGui.getWindowPosY(), 0, 0);
        rect.z = rect.x + ImGui.getWindowWidth();
        rect.w = rect.y + ImGui.getWindowHeight();

        if (ImGui.isMouseHoveringRect(rect.x, rect.y, rect.z, rect.w)) {
            boolean maximized = editor.window.isMaximized();

            if (ImGui.isMouseDoubleClicked(Mouse.Left)) {
                if (maximized) {
                    editor.window.restore();
                }
                else {
                    editor.window.maximize();
                }
                maximized = !maximized;
            }

            if (!maximized && Input.isMouseDown(Mouse.Left)) {
                final Int2 pos = editor.window.getPosition();
                final ImVec2 drag = ImGui.getMouseDragDelta();
                editor.window.setPosition(new Int2(pos.x + (int) drag.x, pos.y + (int) drag.y));
            }
        }
    }

    private void fileMenu() {
        if (ImGui.beginMenu("File")) {
            if (ImGui.menuItem("New scene")) {
                editor.eraseScene();
            }

            if (ImGui.menuItem("Save scene")) {
                textInput = new GUITextInput(FileHelper.defaultPath() + FileHelper.separator, path -> {
                    final String fullPath = path + ".titian";
                    if (editor.getScene().toFile(fullPath)) {
                        System.out.println("Scene \"" + fullPath + "\" saved!");
                    }
                    textInput = null;
                });
            }

            if (ImGui.menuItem("Exit")) {
                editor.window.close();
            }

            ImGui.endMenu();
        }
    }

    private void editMenu() {
        if (ImGui.beginMenu("Edit")) {
            if (ImGui.menuItem("Reload scripts")) {
                for (Entity entity : editor.getScene()) {
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
                GUIDisplay.texture(editor.editorRenderer.renderBuffer.getDepthMap(), size);
                ImGui.endMenu();
            }

            if (ImGui.beginMenu("Index Texture")) {
                GUIDisplay.texture(editor.editorRenderer.indexBuffer.getColorMap(), size);
                ImGui.endMenu();
            }

            ImGui.endMenu();
        }
    }

    private float barButtonSizes = 0;
    private void barButtons() {
        ImGui.setCursorPosX(ImGui.getWindowPosX() + ImGui.getWindowWidth() - barButtonSizes);
        barButtonSizes = 0;

        if (ImGui.menuItem("  --  ")) {
            editor.window.minimize();
        }
        barButtonSizes += ImGui.getItemRectSizeX();

        if (ImGui.menuItem("  O  ")) {
            if (editor.window.isMaximized()) {
                editor.window.restore();
            }
            else {
                editor.window.maximize();
            }
        }
        barButtonSizes += ImGui.getItemRectSizeX();

        if (ImGui.menuItem("  X  ")) {
            editor.window.close();
        }
        barButtonSizes += ImGui.getItemRectSizeX();
    }

    @Override
    public void renderGUI() {
        ImGui.pushStyleVar(ImGuiStyleVar.FramePadding, 0, 8);

        if (ImGui.beginMainMenuBar()) {
            windowControl();

            ImGui.text("TITIAN");

            fileMenu();
            editMenu();
            viewMenu();
            renderMenu();

            barButtons();

            if (Instance.isValid(textInput)) {
                textInput.update();
            }
        }
        ImGui.endMainMenuBar();

        ImGui.popStyleVar();
    }
}
