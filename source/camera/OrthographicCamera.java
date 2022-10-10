package camera;

import camera.abs.Camera;
import editor.Editor;
import glparts.Shaders;
import gui.GUIEdit;
import gui.GUIStyle;
import math.Float3;
import math.Mat4;
import named.NameHolder;

import java.io.Serializable;

public class OrthographicCamera extends Camera implements Serializable {
    public float width = 160;
    public float height = 90;

    public OrthographicCamera(NameHolder holder, String name, Editor editor) {
        super(holder, name, editor);
    }

    @Override
    public Mat4 projectionMatrix() {
        return Mat4.orthographic(width, height, near, far);
    }

    @Override
    public Mat4 matrix() {
        return projectionMatrix().multiply(viewMatrix());
    }

    @Override
    public void renderInfoGUI(Editor editor) {
        super.renderInfoGUI(editor);

        width = GUIEdit.editFloat("Width", width, 0.1f);
        height = GUIEdit.editFloat("Height", height, 0.1f);
    }

    @Override
    public void editorRender(Shaders shaders) {
        editor.data.frustumShaders.setUniform("iVP", matrix().inverse());
        editor.data.frustumShaders.setUniform("VP", editor.camera.matrix());
        editor.data.frustumShaders.setUniform("color", (this == editor.scene.selected) ? GUIStyle.special : new Float3(1));
        editor.data.frustumMesh.renderLines(editor.data.frustumShaders);

        super.editorRender(shaders);
    }
}
