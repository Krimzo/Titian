package scene;

import camera.abs.Camera;
import entity.Entity;
import light.AmbientLight;
import light.DirectionalLight;

import java.io.Serializable;

public final class SceneSelectedData implements Serializable {
    public Entity entity = null;
    public Camera camera = null;
    public AmbientLight ambientLight = null;
    public DirectionalLight directionalLight = null;

    public SceneSelectedData() {}
}
