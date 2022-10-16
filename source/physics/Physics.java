package physics;

import scene.Scene;

public class Physics {
    public Physics() {}

    public void update(Scene scene, float deltaT) {
        for (Physical physical : scene) {
            physical.updatePhysics(deltaT);
        }
    }
}
