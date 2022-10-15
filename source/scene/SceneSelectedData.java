package scene;

import camera.abs.Camera;
import entity.Entity;
import light.AmbientLight;
import light.DirectionalLight;
import utility.abs.Allocated;

import java.io.Serializable;

public class SceneSelectedData implements Allocated, Serializable {
    public Entity entity = null;
    public Camera camera = null;
    public AmbientLight ambientLight = null;
    public DirectionalLight directionalLight = null;

    public SceneSelectedData() {}

    @Override
    public void free() {
        entity = null;
        camera = null;
        ambientLight = null;
        directionalLight = null;
    }
}
