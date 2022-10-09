package gui.section;

import editor.Editor;
import gui.GUIUtil;
import gui.abs.GUISection;
import imgui.ImGui;
import imgui.flag.ImGuiCol;
import logging.Logger;

public final class GUILogView extends GUISection {
    public GUILogView(Editor editor) {
        super(editor);
    }

    @Override
    public void renderGUI() {
        Logger logger = editor.logger;
        int unseenCount = logger.getUnseenCount();

        if (ImGui.begin("Log View" + (unseenCount > 0 ? (" [" + unseenCount + "]###") : "###"))) {
            ImGui.bulletText(logger.getLastClearMessage());
            ImGui.separator();

            logger.iterate(log -> {
                GUIUtil.useColor3(ImGuiCol.Text, log.type.toColor(), () -> {
                    ImGui.text(log.firstMessagePart + logger.formatSpaces(log) + log.secondMessagePart);
                });
            });
            logger.clearUnseen();

            if (ImGui.beginPopupContextWindow("EditLogView")) {
                if (ImGui.button("Clear")) {
                    logger.clear();

                    ImGui.closeCurrentPopup();
                }

                ImGui.endPopup();
            }
        }
        ImGui.end();
    }
}
