package gui.section;

import editor.Editor;
import gui.GUIUtil;
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
            ImGui.bulletText(editor.logger.getLastClearMessage());
            ImGui.separator();

            editor.logger.iterate(log -> {
                GUIUtil.useColor3(ImGuiCol.Text, log.type.toColor(), () -> {
                    ImGui.text(log.firstMessagePart + editor.logger.formatSpaces(log) + log.secondMessagePart);
                });
            });

            if (ImGui.beginPopupContextWindow("EditLogView")) {
                if (ImGui.button("Clear")) {
                    editor.logger.clear();

                    ImGui.closeCurrentPopup();
                }

                ImGui.endPopup();
            }
        }
        ImGui.end();
    }
}
