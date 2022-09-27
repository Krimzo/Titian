package gui.sections;

import editor.*;
import gui.*;
import imgui.*;
import imgui.flag.*;
import math.*;

public final class GUIViewport extends GUISection {
    public GUIViewport(Editor editor) {
        super(editor);
    }

    private Int2[] saveViewportInfo() {
        int tabSize = (int) ImGui.getWindowContentRegionMinY();
        Int2 position = new Int2((int) ImGui.getWindowPosX(), (int) ImGui.getWindowPosY() + tabSize);
        Int2 size = new Int2((int) ImGui.getWindowWidth(), (int) ImGui.getWindowHeight() - tabSize);

        editor.savedData.put("ViewportPosition", position);
        editor.savedData.put("ViewportSize", size);

        return new Int2[] { position, size };
    }

    private void objectSelection(Int2 viewportPosition, Int2 viewportSize) {
        if (editor.getScene() == null) {
            return;
        }

        Float4 viewportRect = new Float4(
            viewportPosition.x, viewportPosition.y, viewportPosition.x + viewportSize.x, viewportPosition.y + viewportSize.y
        );

        if (ImGui.isMouseHoveringRect(viewportRect.x, viewportRect.y, viewportRect.z, viewportRect.w) && ImGui.isMouseClicked(ImGuiMouseButton.Left)) {
            Float2 mousePosition = new Float2(ImGui.getMousePosX() - viewportPosition.x, ImGui.getMousePosY() - viewportPosition.y);

            int objectIndex = (int) editor.renderer.indexBuffer.getPixel(new Int2(mousePosition)).x - 1;
            editor.getScene().selectedEntity = (objectIndex < 0) ? null : editor.getScene().get(objectIndex);
        }
    }

    public void displayFrame(Int2 viewportSize) {
        ImGui.image(editor.renderer.renderBuffer.getColorMap().getID(), viewportSize.x, viewportSize.y, 0, 1, 1, 0);
    }

    @Override
    public void onGUIRender() {
        ImGui.pushStyleVar(ImGuiStyleVar.WindowPadding, 0, 0);

        if (ImGui.begin("Viewport", ImGuiWindowFlags.NoScrollbar)) {
            Int2[] viewportInfo = saveViewportInfo();
            objectSelection(viewportInfo[0], viewportInfo[1]);
            displayFrame(viewportInfo[1]);
        }
        ImGui.end();

        ImGui.popStyleVar();
    }
}
