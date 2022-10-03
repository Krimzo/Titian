package renderer;

import glparts.*;
import math.*;
import renderer.abs.Renderable;
import scene.Scene;
import glparts.abs.Disposable;
import window.*;

import java.util.HashMap;
import java.util.Map;

import static org.lwjgl.opengl.GL33.*;

public class Renderer implements Disposable {
    public final Camera camera = new Camera();
    public final FrameBuffer renderBuffer;
    public final FrameBuffer indexBuffer;

    private final Shaders renderShaders;
    private final Shaders indexShaders;
    private final Shaders outlineShaders;

    public final Map<String, Mesh> predefinedMeshes = new HashMap<>();

    public Renderer(GLContext context, Int2 size) {
        renderBuffer = new FrameBuffer(context, size);
        indexBuffer = new FrameBuffer(context, size);
        indexBuffer.getColorMap().setWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

        renderShaders = new Shaders(context, "shaders/Render.glsl");
        indexShaders = new Shaders(context, "shaders/Index.glsl");
        outlineShaders = new Shaders(context, "shaders/Outline.glsl");

        predefinedMeshes.put("ScreenMesh", Mesh.generateScreenMesh());
    }

    @Override
    public void dispose() {
        for (var obj : predefinedMeshes.entrySet()) {
            obj.getValue().dispose();
        }
        predefinedMeshes.clear();
    }

    public void resize(Int2 size) {
        renderBuffer.resize(size);
        indexBuffer.resize(size);
        camera.updateAspect(size);
    }

    public void renderIndices(Scene scene) {
        indexBuffer.context.setClearColor(new Float4(0));
        indexBuffer.clear();

        indexBuffer.use(() -> {
            indexShaders.setUniform("VP", camera.matrix());
            for (int i = 0; i < scene.size(); i++) {
                indexShaders.setUniform("index", i + 1);
                scene.get(i).render(indexShaders);
            }
        });
    }

    public void renderScene(Scene scene) {
        renderBuffer.context.setClearColor(new Float4(0.2f));
        renderBuffer.clear();

        renderBuffer.use(() -> {
            renderShaders.setUniform("VP", camera.matrix());
            for (Renderable renderable : scene) {
                renderable.render(renderShaders);
            }
        });
    }

    public void renderOutline(Float2 frameSize, Float4 outlineColor, int objectIndex) {
        indexBuffer.getColorMap().use(0, () -> {
            renderBuffer.context.setDepthTest(false);
            renderBuffer.use(() -> {
                outlineShaders.setUniform("indexMap", 0);
                outlineShaders.setUniform("outlineColor", outlineColor);
                outlineShaders.setUniform("frameSize", frameSize);
                outlineShaders.setUniform("outlineThickness", 1);
                outlineShaders.setUniform("selectedIndex", objectIndex + 1);
                predefinedMeshes.get("ScreenMesh").render(outlineShaders);
            });
            renderBuffer.context.setDepthTest(true);
        });
    }
}
