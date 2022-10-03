package gui.section;

import editor.Editor;
import gui.GUIRenderer;
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
            editor.logger.iterate(log -> {
                GUIRenderer.useColor(ImGuiCol.Text, log.type.toColor(), () -> {
                    ImGui.text(editor.logger.convertInfo(log));
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
