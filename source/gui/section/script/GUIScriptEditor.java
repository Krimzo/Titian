package gui.section.script;

import editor.Editor;
import gui.abs.GUISection;
import gui.helper.GUIDragDrop;
import imgui.ImGui;
import imgui.extension.texteditor.TextEditor;
import imgui.extension.texteditor.TextEditorLanguageDefinition;
import imgui.flag.ImGuiStyleVar;
import imgui.flag.ImGuiWindowFlags;
import utility.Instance;
import utility.helper.FileHelper;
import window.input.Input;
import window.input.Key;

import java.util.HashMap;
import java.util.Map;

public final class GUIScriptEditor extends GUISection {
    private final TextEditor textEditor = new TextEditor();
    private final int[] darkPalette;

    private String filepath = null;
    private float lastMenuTextSize = 0;

    public GUIScriptEditor(Editor editor) {
        super(editor);

        TextEditorLanguageDefinition languageDefinition = TextEditorLanguageDefinition.cPlusPlus();

        languageDefinition.setName("Java");
        languageDefinition.setKeywords(LanguageData.getKeywords());

        Map<String, String> identifiers = new HashMap<>();
        for (String val : LanguageData.getIdentifiers()) {
            identifiers.put(val, "");
        }
        languageDefinition.setIdentifiers(identifiers);

        textEditor.setLanguageDefinition(languageDefinition);

        darkPalette = ColorPalette.getCustomDark(textEditor);
        textEditor.setPalette(darkPalette);

        textEditor.setTabSize(4);
        textEditor.setShowWhitespaces(false);
        textEditor.setColorizerEnable(true);
    }

    private void saveFileText() {
        FileHelper.writeString(filepath, textEditor.getText().replaceAll("\t", " ".repeat(4)));
    }

    private void loadFileText() {
        textEditor.setText(FileHelper.readString(filepath));
    }

    private void handleInput() {
        if (Input.isCtrlDown()) {
            if (Input.isKeyPressed(Key.S)) {
                saveFileText();
            }
            if (Input.isKeyPressed(Key.R)) {
                loadFileText();
            }
        }
    }

    private void renderMenu() {
        ImGui.pushStyleVar(ImGuiStyleVar.WindowPadding, 10, 10);

        if (ImGui.beginMenuBar()) {
            if (ImGui.beginMenu("File")) {
                if (ImGui.menuItem("Save", "Ctrl-S")) {
                    saveFileText();
                }

                if (ImGui.menuItem("Reload")) {
                    loadFileText();
                }

                if (ImGui.menuItem("Close")) {
                    filepath = null;
                    textEditor.setText("");
                }

                ImGui.endMenu();
            }

            if (ImGui.beginMenu("Edit")) {
                if (ImGui.menuItem("Undo", "Ctrl-Z", false, textEditor.canUndo())) {
                    textEditor.undo(1);
                }
                if (ImGui.menuItem("Redo", "Ctrl-Y", false, textEditor.canRedo())) {
                    textEditor.redo(1);
                }

                ImGui.separator();

                if (ImGui.menuItem("Copy", "Ctrl-C", false, textEditor.hasSelection())) {
                    textEditor.copy();
                }
                if (ImGui.menuItem("Cut", "Ctrl-X", false, textEditor.hasSelection())) {
                    textEditor.cut();
                }
                if (ImGui.menuItem("Paste", "Ctrl-V", false, ImGui.getClipboardText() != null)) {
                    textEditor.paste();
                }

                ImGui.endMenu();
            }

            if (ImGui.beginMenu("View")) {
                if (ImGui.menuItem("Dark palette")) {
                    textEditor.setPalette(darkPalette);
                }
                if (ImGui.menuItem("Light palette")) {
                    textEditor.setPalette(textEditor.getLightPalette());
                }

                ImGui.endMenu();
            }

            ImGui.setCursorPosX(ImGui.getWindowWidth() * 0.5f - lastMenuTextSize * 0.5f);
            ImGui.text(Instance.isValid(this.filepath) ? this.filepath : "Unknown");
            lastMenuTextSize = ImGui.getItemRectSizeX();

            ImGui.endMenuBar();
        }

        ImGui.popStyleVar();
    }

    private void renderTextEditor() {
        ImGui.pushFont(editor.guiRenderer.fonts.scriptEditorFont);

        textEditor.render("Script Editor");

        GUIDragDrop.getData("ScriptFile", filepath -> {
            this.filepath = (String) filepath;
            loadFileText();
        });

        ImGui.popFont();
    }

    @Override
    public void renderGUI() {
        ImGui.pushStyleVar(ImGuiStyleVar.WindowPadding, 1, 1);

        if (ImGui.begin("Script Editor##Window", ImGuiWindowFlags.MenuBar)) {
            handleInput();
            renderMenu();
            renderTextEditor();
        }
        ImGui.end();

        ImGui.popStyleVar();
    }
}