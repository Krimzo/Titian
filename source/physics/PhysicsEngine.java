package physics;

import physics.abs.Physical;
import physx.PxTopLevelFunctions;
import physx.common.PxDefaultErrorCallback;
import physx.common.PxFoundation;
import physx.common.PxTolerancesScale;
import physx.extensions.PxDefaultAllocator;
import physx.physics.PxPhysics;
import scene.Scene;

public class PhysicsEngine {
    private final PxDefaultAllocator allocator;
    private final PxDefaultErrorCallback errorCallback;
    private final PxFoundation foundation;
    private final PxTolerancesScale tolerances;
    private final PxPhysics physics;

    public PhysicsEngine() {
        final int version = PxTopLevelFunctions.getPHYSICS_VERSION();
        allocator = new PxDefaultAllocator();
        errorCallback = new PxDefaultErrorCallback();
        foundation = PxTopLevelFunctions.CreateFoundation(version, allocator, errorCallback);
        tolerances = new PxTolerancesScale();
        physics = PxTopLevelFunctions.CreatePhysics(version, foundation, tolerances);
    }

    @Override
    protected void finalize() {
        physics.release();
        tolerances.destroy();
        foundation.release();
        errorCallback.destroy();
        allocator.destroy();
    }

    public void update(Scene scene, float deltaT) {
        for (Physical physical : scene) {
            physical.updatePhysics(deltaT);
        }
    }
}
