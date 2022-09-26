package renderer;

import glparts.*;
import math.Int2;
import window.GLContext;

import java.util.ArrayList;

public class Renderer extends ArrayList<Renderable> {
    public FrameBuffer frameBuffer;
    public Shaders shaders;
    public Camera camera = new Camera();

    public Renderer(GLContext context, Int2 size) throws Exception {
        frameBuffer = new FrameBuffer(context, size);
        shaders = new Shaders(context, "shaders/Render.glsl");
    }

    public void updateVPMatrix() {
        if (shaders != null && camera != null) {
            shaders.setUniform("VP", camera.matrix());
        }
    }

    public void updateSize(Int2 newSize) {
        frameBuffer.updateSize(newSize);
        camera.updateAspect(newSize);
    }

    public void render() {
        frameBuffer.bind();
        frameBuffer.clear(true);

        updateVPMatrix();

        for (Renderable renderable : this) {
            renderable.onRender(shaders);
        }

        frameBuffer.unbind();
    }
}
