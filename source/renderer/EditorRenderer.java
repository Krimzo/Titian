package renderer;

import camera.abs.Camera;
import glparts.*;
import math.*;
import renderer.abs.EditorRenderable;
import scene.Scene;
import glparts.abs.Disposable;
import window.*;

import java.util.HashMap;
import java.util.Map;

import static org.lwjgl.opengl.GL33.*;

public class EditorRenderer implements Disposable {
    public final FrameBuffer renderBuffer;
    public final FrameBuffer indexBuffer;

    private final Shaders renderShaders;
    private final Shaders indexShaders;
    private final Shaders outlineShaders;

    public final Map<String, Mesh> predefinedMeshes = new HashMap<>();

    public EditorRenderer(GLContext context, Int2 size) {
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

        outlineShaders.dispose();
        indexShaders.dispose();
        renderShaders.dispose();

        indexBuffer.dispose();
        renderBuffer.dispose();
    }

    public void resize(Int2 size) {
        renderBuffer.resize(size);
        indexBuffer.resize(size);
    }

    public void clear(Camera camera) {
        renderBuffer.context.setClearColor(new Float4(camera.background, 1));
        renderBuffer.clear();
    }

    public void renderIndices(Scene scene, Camera camera) {
        indexBuffer.context.setClearColor(new Float4(0));
        indexBuffer.clear();

        indexBuffer.use(() -> {
            indexShaders.setUniform("VP", camera.matrix());
            for (int i = 0; i < scene.size(); i++) {
                indexShaders.setUniform("index", i + 1);
                scene.get(i).indexRender(indexShaders);
            }
        });
    }

    public void renderScene(Scene scene, Camera camera) {
        renderBuffer.use(() -> {
            renderShaders.setUniform("VP", camera.matrix());
            for (EditorRenderable renderable : scene) {
                renderable.editorRender(renderShaders);
            }
        });
    }

    public void renderOutline(Float2 frameSize, Float3 outlineColor, int objectIndex) {
        indexBuffer.getColorMap().use(0, () -> {
            renderBuffer.context.setDepthTest(false);
            renderBuffer.use(() -> {
                outlineShaders.setUniform("indexMap", 0);
                outlineShaders.setUniform("outlineColor", new Float4(outlineColor, 1));
                outlineShaders.setUniform("frameSize", frameSize);
                outlineShaders.setUniform("outlineThickness", 1);
                outlineShaders.setUniform("selectedIndex", objectIndex + 1);
                predefinedMeshes.get("ScreenMesh").renderTriangles(outlineShaders);
            });
            renderBuffer.context.setDepthTest(true);
        });
    }
}
