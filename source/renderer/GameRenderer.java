package renderer;

import camera.abs.Camera;
import glparts.FrameBuffer;
import glparts.Shaders;
import glparts.abs.Disposable;
import math.Float3;
import math.Float4;
import math.Int2;
import renderer.abs.GameRenderable;
import scene.Scene;
import glparts.abs.GLContext;

public class GameRenderer implements Disposable {
    public final FrameBuffer renderBuffer;
    private final Shaders renderShaders;

    public GameRenderer(GLContext context, Int2 size) {
        renderBuffer = new FrameBuffer(context, size);
        renderShaders = new Shaders(context, "shaders/Render.glsl");
    }

    @Override
    public void dispose() {
        renderShaders.dispose();
        renderBuffer.dispose();
    }

    public void resize(Int2 size) {
        renderBuffer.resize(size);
    }

    public void clear(Camera camera) {
        Float3 color = (camera != null) ? camera.background : new Float3(0.1f);
        renderBuffer.context.setClearColor(new Float4(color, 1));
        renderBuffer.clear();
    }

    public void renderScene(Scene scene, Camera camera) {
        renderBuffer.use(() -> {
            renderShaders.setUniform("VP", camera.matrix());
            for (GameRenderable renderable : scene) {
                renderable.gameRender(renderShaders);
            }
        });
    }
}
