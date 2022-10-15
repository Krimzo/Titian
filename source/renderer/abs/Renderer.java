package renderer.abs;

import camera.abs.Camera;
import math.Int2;
import scene.Scene;

public abstract class Renderer {
    public Renderer() {}

    public abstract void resize(Int2 size);

    public abstract void clear(Camera camera);

    protected abstract void renderRenderable(Renderable renderable);

    public abstract void renderScene(Scene scene, Camera camera);
}
