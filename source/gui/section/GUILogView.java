package gui.section;

import editor.Editor;
import gui.GUI;
import gui.abs.GUISection;
import imgui.ImGui;
import imgui.flag.ImGuiCol;

public final class GUILogView extends GUISection {
    public GUILogView(Editor editor) {
        super(editor);
    }

    @Override
    public void renderGUI() {
        if (ImGui.begin("Log View")) {
            ImGui.text(editor.logger.getLastClearMessage());
            ImGui.separator();

            editor.logger.iterate(log -> {
                GUI.useColor(ImGuiCol.Text, log.type.toColor(), () -> {
                    ImGui.text(log.firstMessagePart + editor.logger.formatSpaces(log) + log.secondMessagePart);
                });
            });

            if (ImGui.beginPopupContextWindow("Edit")) {
                if (ImGui.selectable("Clear")) {
                    editor.logger.clear();
                }
                ImGui.endPopup();
            }
        }
        ImGui.end();
    }
}
