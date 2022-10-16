package gui.helper;

import glparts.DepthTexture;
import glparts.Texture;
import imgui.ImGui;
import math.Float2;
import math.Int2;

public final class GUIDisplay {
    private GUIDisplay() {}

    public static void texture(Texture texture, Int2 size) {
        GUIDisplay.texture(texture, new Float2(size));
    }

    public static void texture(DepthTexture texture, Int2 size) {
        GUIDisplay.texture(texture, new Float2(size));
    }

    public static void texture(Texture texture, Float2 size) {
        GUIDisplay.texture(texture.getBuffer(), size.x, size.y);
    }

    public static void texture(DepthTexture texture, Float2 size) {
        GUIDisplay.texture(texture.getBuffer(), size.x, size.y);
    }

    public static void texture(int texture, float width, float height) {
        ImGui.image(texture, width, height, 0, 1, 1, 0);
    }
}
