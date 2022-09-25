package physics;

import java.util.ArrayList;

public class Physics extends ArrayList<Physical> {
    public Physics() {}

    public void update(float deltaT) {
        for (Physical physical : this) {
            physical.onPhysicsUpdate(deltaT);
        }
    }
}
