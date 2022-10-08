package camera;

import camera.abs.Camera;
import editor.Editor;
import glparts.Shaders;
import gui.GUIStyle;
import gui.GUIUtil;
import math.*;
import named.NameHolder;

import java.io.Serializable;

public class PerspectiveCamera extends Camera implements Serializable {
    public float aspect = 1.78f;
    public float fov = 90;

    public PerspectiveCamera(NameHolder holder, String name, Editor editor) {
        super(holder, name, editor);
    }

    public void updateAspect(Int2 frameSize) {
        aspect = ((float) frameSize.x) / frameSize.y;
    }

    @Override
    public Mat4 viewMatrix() {
        return Mat4.lookAt(transformComponent.position, transformComponent.position.add(forward), Float3.getPosY());
    }

    @Override
    public Mat4 projectionMatrix() {
        return Mat4.perspective(fov, aspect, near, far);
    }

    @Override
    public Mat4 matrix() {
        return projectionMatrix().multiply(viewMatrix());
    }

    @Override
    public void renderInfoGUI(Editor editor) {
        super.renderInfoGUI(editor);

        aspect = GUIUtil.editFloat("Aspect", aspect, 0.01f);
        fov = GUIUtil.editFloat("FOV", fov, 0.1f);
    }

    @Override
    public void editorRender(Shaders shaders) {
        editor.data.frustumShaders.setUniform("iVP", matrix().inverse());
        editor.data.frustumShaders.setUniform("VP", editor.camera.matrix());
        editor.data.frustumShaders.setUniform("color", (this == editor.scene.selectedEntity) ? GUIStyle.special : new Float3(1));
        editor.data.frustumMesh.renderLines(editor.data.frustumShaders);

        super.editorRender(shaders);
    }
}
