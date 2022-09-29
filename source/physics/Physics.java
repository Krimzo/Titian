package physics;

import scene.Scene;

public class Physics {
    public Physics() {}

    public void update(Scene scene, float deltaT) {
        if (scene != null) {
            for (Physical physical : scene) {
                physical.updatePhysics(deltaT);
            }
        }
    }
}
