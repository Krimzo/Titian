package physics;

import scene.Scene;
import utility.Instance;

public class Physics {
    public Physics() {}

    public void update(Scene scene, float deltaT) {
        if (Instance.isValid(scene)) {
            for (Physical physical : scene) {
                physical.updatePhysics(deltaT);
            }
        }
    }
}
