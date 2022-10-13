package light;

import editor.Editor;
import gui.helper.GUIEdit;
import imgui.ImGui;
import light.abs.Light;
import math.Float3;
import named.NameHolder;

import java.io.Serializable;

public class DirectionalLight extends Light implements Serializable {
    private final Float3 direction = Float3.getNegZ();

    public DirectionalLight(NameHolder holder, String name, Editor editor) {
        super(holder, name, editor);
    }

    public void setDirection(Float3 direction) {
        this.direction.set(direction.normalize());
    }

    public Float3 getDirection() {
        return new Float3(direction);
    }

    @Override
    public void renderInfoGUI(Editor editor) {
        super.renderInfoGUI(editor);

        GUIEdit.editFloat3("Direction", direction, 0.1f);
        direction.set(direction.normalize());

        boolean isMain = editor.scene.selected.directionalLight == this;
        if (ImGui.checkbox("Main directional light", isMain)) {
            editor.scene.selected.directionalLight = isMain ? null : this;
        }
    }
}
