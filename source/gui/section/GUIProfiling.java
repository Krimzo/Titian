package gui.section;

import editor.Editor;
import gui.abs.GUISection;
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

            ImGui.dragFloat("FPS", new float[]{ 1 / deltaTime });
            ImGui.dragFloat("Frame", new float[]{ deltaTime });
        }
        ImGui.end();

        ImGui.popStyleVar();
    }
}
