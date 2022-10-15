package gui.section;

import editor.Editor;
import gui.abs.GUISection;
import gui.helper.GUIEdit;
import gui.helper.GUIPopup;
import imgui.ImGui;
import imgui.flag.ImGuiCol;
import logging.Logger;

public final class GUILogView extends GUISection {
    public GUILogView(Editor editor) {
        super(editor);
    }

    @Override
    public void free() {

    }

    @Override
    public void renderGUI() {
        Logger logger = editor.logger;
        int unseenCount = logger.getUnseenCount();

        if (ImGui.begin("Log View" + (unseenCount > 0 ? (" [" + unseenCount + "]###") : "###"))) {
            ImGui.bulletText(logger.getLastClearMessage());
            ImGui.separator();

            logger.iterate(log -> {
                GUIEdit.useColor3(ImGuiCol.Text, log.type.toColor(), () -> {
                    ImGui.text(log.firstMessagePart + logger.formatSpaces(log) + log.secondMessagePart);
                });
            });
            logger.clearUnseen();

            GUIPopup.windowPopup("EditLogView", () -> {
                if (ImGui.button("Clear")) {
                    logger.clear();
                    GUIPopup.close();
                }
            });
        }
        ImGui.end();
    }
}
