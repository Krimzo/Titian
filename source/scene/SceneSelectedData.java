package scene;

import camera.abs.Camera;
import entity.Entity;
import glparts.abs.Disposable;
import light.AmbientLight;
import light.DirectionalLight;

import java.io.Serializable;

public class SceneSelectedData implements Disposable, Serializable {
    public Entity entity = null;
    public Camera camera = null;
    public AmbientLight ambientLight = null;
    public DirectionalLight directionalLight = null;

    public SceneSelectedData() {}

    @Override
    public void dispose() {
        entity = null;
        camera = null;
        ambientLight = null;
        directionalLight = null;
    }
}
