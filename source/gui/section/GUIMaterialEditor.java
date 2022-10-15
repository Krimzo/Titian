package gui.section;

import editor.Editor;
import gui.abs.GUISection;
import gui.helper.GUITextInput;
import imgui.ImGui;
import utility.Instance;

public final class GUIMaterialEditor extends GUISection {
    private GUITextInput textInput = null;

    public GUIMaterialEditor(Editor editor) {
        super(editor);
    }

    @Override
    public void free() {

    }

    @Override
    public void renderGUI() {
        if (ImGui.begin("Material Editor")) {
            ImGui.columns(2);
            if (ImGui.beginChild("Materials")) {

            }
            ImGui.endChild();

            ImGui.nextColumn();
            if (ImGui.beginChild("Material View")) {

            }
            ImGui.endChild();

            if (Instance.isValid(textInput)) {
                textInput.update();
            }
        }
        ImGui.end();
    }
}