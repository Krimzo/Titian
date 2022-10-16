package gui.section;

import editor.Editor;
import gui.abs.GUISection;
import gui.helper.GUIEdit;
import imgui.ImGui;
import imgui.flag.ImGuiStyleVar;

public final class GUIProfiling extends GUISection {
    private boolean vSync = true;

    public GUIProfiling(Editor editor) {
        super(editor);
    }

    @Override
    public void renderGUI() {
        ImGui.pushStyleVar(ImGuiStyleVar.WindowPadding, 14, 14);

        if (ImGui.begin("Profiling")) {
            final float deltaTime = editor.timer.getDeltaT();

            if (ImGui.checkbox("vSync", vSync)) {
                editor.window.setVSync(vSync = !vSync);
            }

            GUIEdit.editFloat("FPS", 1 / deltaTime, 0);
            GUIEdit.editFloat("Frame", deltaTime, 0);
        }
        ImGui.end();

        ImGui.popStyleVar();
    }
}
