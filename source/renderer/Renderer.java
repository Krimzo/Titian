package renderer;

import glparts.*;

import java.util.ArrayList;

public class Renderer extends ArrayList<Renderable> {
    public Shaders shaders = null;
    public Camera camera = new Camera();

    public Renderer() {}

    public void updateVPMatrix() {
        if (shaders != null && camera != null) {
            shaders.setUniform("VP", camera.matrix());
        }
    }

    public void render() {
        for (Renderable renderable : this) {
            renderable.onRender(shaders);
        }
    }
}
