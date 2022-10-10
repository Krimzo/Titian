package renderer;

import camera.abs.Camera;
import glparts.FrameBuffer;
import glparts.Mesh;
import glparts.Shaders;
import glparts.abs.Disposable;
import glparts.abs.GLContext;
import math.Float2;
import math.Float3;
import math.Float4;
import math.Int2;
import renderer.abs.Renderable;
import scene.Scene;

import static org.lwjgl.opengl.GL33.GL_CLAMP_TO_EDGE;

public class EditorRenderer extends GameRenderer implements Disposable {
    public final FrameBuffer indexBuffer;
    private final Shaders indexShaders;
    private final Shaders outlineShaders;
    private final Mesh screenMesh;

    public EditorRenderer(GLContext context, Int2 size) {
        super(context, size);

        indexBuffer = new FrameBuffer(context, size);
        indexBuffer.getColorMap().setWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
        indexShaders = new Shaders(context, "shaders/Index.glsl");
        outlineShaders = new Shaders(context, "shaders/Outline.glsl");
        screenMesh = Mesh.generateScreenMesh();
    }

    @Override
    public void dispose() {
        super.dispose();

        indexBuffer.dispose();
        indexShaders.dispose();
        outlineShaders.dispose();
        screenMesh.dispose();
    }

    @Override
    public void resize(Int2 size) {
        super.resize(size);

        indexBuffer.resize(size);
    }

    @Override
    protected void renderRenderable(Renderable renderable) {
        renderable.editorRender(renderShaders);
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

    public void renderOutline(Float2 frameSize, Float3 outlineColor, int objectIndex) {
        indexBuffer.getColorMap().use(0, () -> {
            renderBuffer.context.setDepthTest(false);

            renderBuffer.use(() -> {
                outlineShaders.setUniform("indexMap", 0);
                outlineShaders.setUniform("outlineColor", new Float4(outlineColor, 1));
                outlineShaders.setUniform("frameSize", frameSize);
                outlineShaders.setUniform("outlineThickness", 1);
                outlineShaders.setUniform("selectedIndex", objectIndex + 1);

                screenMesh.renderTriangles(outlineShaders);
            });

            renderBuffer.context.setDepthTest(true);
        });
    }
}
