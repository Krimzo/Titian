package gui.sections;

import editor.Editor;
import gui.GUIRenderable;
import gui.GUISection;
import imgui.ImGui;
import imgui.flag.ImGuiStyleVar;
import imgui.flag.ImGuiWindowFlags;
import math.Int2;

public final class GUIViewport extends GUISection implements GUIRenderable {
    public GUIViewport(Editor editor) {
        super(editor);
    }

    private Int2 saveViewportSize() {
        Int2 viewportSize = new Int2((int) ImGui.getWindowWidth(), (int) ImGui.getWindowHeight());
        editor.savedData.put("ViewportSize", viewportSize);
        return viewportSize;
    }

    @Override
    public void onGUIRender() {
        ImGui.pushStyleVar(ImGuiStyleVar.WindowPadding, 0, 0);

        if (ImGui.begin("Viewport", ImGuiWindowFlags.NoScrollbar)) {
            Int2 viewportSize = saveViewportSize();
            ImGui.image(editor.renderer.frameBuffer.getColorMap().getID(), viewportSize.x, viewportSize.y, 0, 1, 1, 0);
        }
        ImGui.end();

        ImGui.popStyleVar();
    }
}
