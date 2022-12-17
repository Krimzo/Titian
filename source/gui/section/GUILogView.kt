package gui.section

import editor.Editor
import gui.abs.GUISection
import gui.helper.GUIEdit
import gui.helper.GUIPopup
import imgui.ImGui
import imgui.flag.ImGuiCol
import logging.LogInfo

class GUILogView(editor: Editor) : GUISection(editor) {
    override fun renderGUI() {
        val logger = editor.logger
        val unseenCount = logger.getUnseenCount()

        if (ImGui.begin("Log View" + if (unseenCount > 0) " [$unseenCount]###" else "###")) {
            ImGui.bulletText(logger.getLastClearMessage())
            ImGui.separator()
            logger.iterate { log: LogInfo ->
                GUIEdit.useColor3(ImGuiCol.Text, log.type.toColor()) {
                    ImGui.text(log.firstMessagePart + logger.formatSpaces(log) + log.secondMessagePart)
                }
            }

            logger.clearUnseen()
            GUIPopup.windowPopup("EditLogView") {
                if (ImGui.button("Clear")) {
                    logger.clear()
                    GUIPopup.close()
                }
            }
        }
        ImGui.end()
    }
}