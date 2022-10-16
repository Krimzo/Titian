package gui.helper;

import callback.StringCallback;
import imgui.ImGui;
import imgui.flag.ImGuiInputTextFlags;
import imgui.flag.ImGuiWindowFlags;
import imgui.type.ImString;

public class GUITextInput {
    private static int idCounter = 0;
    private final int id;

    private final StringCallback onEdit;
    private final ImString text;

    public GUITextInput(String text, StringCallback onEdit) {
        this(text, 30, onEdit);
    }

    public GUITextInput(String text, int bufferSize, StringCallback onEdit) {
        id = idCounter++;
        this.text = new ImString(text, bufferSize);
        this.onEdit = onEdit;
    }

    public void update() {
        if (ImGui.begin("Text input##" + id, ImGuiWindowFlags.NoCollapse | ImGuiWindowFlags.NoScrollbar)) {
            ImGui.setNextItemWidth(-1);
            if (ImGui.inputText("##Input", text, ImGuiInputTextFlags.EnterReturnsTrue)) {
                String text = this.text.toString();
                if (text.length() > 0) {
                    onEdit.method(text);
                }
            }
        }
        ImGui.end();
    }
}
