package gui.sections;

import callback.EmptyCallback;
import editor.Editor;
import gui.GUISection;
import imgui.ImGui;
import imgui.flag.ImGuiCol;
import math.Float4;

public final class GUILogView extends GUISection {
    public GUILogView(Editor editor) {
        super(editor);
    }

    private void setTextColor(Float4 color, EmptyCallback callback) {
        ImGui.pushStyleColor(ImGuiCol.Text, color.x, color.y, color.z, color.w);
        callback.method();
        ImGui.popStyleColor();
    }

    @Override
    public void renderGUI() {
        if (ImGui.begin("Log View")) {
            ImGui.text(editor.logger.getLastClearMessage());
            editor.logger.iterate(log -> {
                setTextColor(log.type.toColor(), () -> {
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
