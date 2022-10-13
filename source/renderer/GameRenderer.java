package renderer;

import camera.abs.Camera;
import glparts.FrameBuffer;
import glparts.Shaders;
import glparts.abs.Disposable;
import glparts.abs.GLContext;
import math.Float3;
import math.Float4;
import math.Int2;
import renderer.abs.Renderable;
import renderer.abs.Renderer;
import scene.Scene;

public class GameRenderer extends Renderer implements Disposable {
    public final FrameBuffer renderBuffer;
    protected final Shaders renderShaders;

    public GameRenderer(GLContext context, Int2 size) {
        renderBuffer = new FrameBuffer(context, size);
        renderShaders = new Shaders(context, "shaders/Render.glsl");
    }

    @Override
    public void dispose() {
        renderShaders.dispose();
        renderBuffer.dispose();
    }

    @Override
    public void resize(Int2 size) {
        renderBuffer.resize(size);
    }

    @Override
    public void clear(Camera camera) {
        Float3 color = (camera != null) ? camera.background : new Float3(0);
        renderBuffer.context.setClearColor(new Float4(color, 1));
        renderBuffer.clear();
    }

    @Override
    protected void renderRenderable(Renderable renderable) {
        renderable.gameRender(renderShaders);
    }

    @Override
    public final void renderScene(Scene scene, Camera camera) {
        renderBuffer.use(() -> {
            renderShaders.setUniform("VP", camera.matrix());
            renderShaders.setUniform("ambientColor", (scene.selected.ambientLight != null) ? scene.selected.ambientLight.getColor() : new Float3());
            renderShaders.setUniform("sunDirection", (scene.selected.directionalLight != null) ? scene.selected.directionalLight.getDirection() : new Float3());
            renderShaders.setUniform("sunColor", (scene.selected.directionalLight != null) ? scene.selected.directionalLight.getColor() : new Float3());

            for (Renderable renderable : scene) {
                renderRenderable(renderable);
            }
        });
    }
}
