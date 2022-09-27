package renderer;

import glparts.*;
import math.*;
import scene.Scene;
import window.*;

public class Renderer {
    public final Camera camera = new Camera();
    public final FrameBuffer renderBuffer;
    public final FrameBuffer indexBuffer;
    private final Shaders renderShaders;
    private final Shaders indexShaders;
    private final Shaders outlineShaders;
    private final Mesh screenMesh;

    public Renderer(GLContext context, Int2 size) throws Exception {
        renderBuffer = new FrameBuffer(context, size);
        indexBuffer = new FrameBuffer(context, size);

        renderShaders = new Shaders(context, "shaders/Render.glsl");
        indexShaders = new Shaders(context, "shaders/Index.glsl");
        outlineShaders = new Shaders(context, "shaders/Outline.glsl");

        screenMesh = new Mesh(context, new Vertex[]{
            new Vertex(new Float3(-1.0f, -1.0f, 0.5f)), new Vertex(new Float3(-1.0f, 1.0f, 0.5f)), new Vertex(new Float3(1.0f, 1.0f, 0.5f)),
            new Vertex(new Float3(1.0f, 1.0f, 0.5f)), new Vertex(new Float3(1.0f, -1.0f, 0.5f)), new Vertex(new Float3(-1.0f, -1.0f, 0.5f))
        });
    }

    public void updateSize(Int2 newSize) {
        renderBuffer.updateSize(newSize);
        indexBuffer.updateSize(newSize);
        camera.updateAspect(newSize);
    }

    public void renderIndices(Scene scene) {
        indexBuffer.context.setClearColor(new Float4(0));
        indexBuffer.clear(true);

        indexShaders.setUniform("VP", camera.matrix());

        for (int i = 0; i < scene.size(); i++) {
            indexShaders.setUniform("index", i + 1);
            scene.get(i).onRender(indexShaders);
        }

        indexBuffer.unbind();
    }

    public void renderScene(Scene scene) {
        renderBuffer.context.setClearColor(new Float4(0.2f));
        renderBuffer.clear(true);

        renderShaders.setUniform("VP", camera.matrix());

        for (Renderable renderable : scene) {
            renderable.onRender(renderShaders);
        }

        renderBuffer.unbind();
    }

    public void renderOutline(Float2 frameSize, Float4 outlineColor, int objectIndex) {
        indexBuffer.getColorMap().bind(0);

        renderBuffer.bind();
        renderBuffer.context.setDepthTest(false);

        outlineShaders.setUniform("indexMap", 0);
        outlineShaders.setUniform("outlineColor", outlineColor);
        outlineShaders.setUniform("frameSize", frameSize);
        outlineShaders.setUniform("outlineThickness", 1);
        outlineShaders.setUniform("selectedIndex", objectIndex + 1);

        screenMesh.onRender(outlineShaders);

        renderBuffer.context.setDepthTest(true);
        renderBuffer.unbind();

        indexBuffer.getColorMap().unbind();
    }
}
