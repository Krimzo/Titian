package renderer.abs;

import camera.abs.Camera;
import math.Int2;
import scene.Scene;
import utility.abs.Allocated;

public abstract class Renderer implements Allocated {
    public Renderer() {}

    public abstract void resize(Int2 size);

    public abstract void clear(Camera camera);

    protected abstract void renderRenderable(Renderable renderable);

    public abstract void renderScene(Scene scene, Camera camera);
}
