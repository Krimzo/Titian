package physics;

import math.Float3;
import physx.PxTopLevelFunctions;
import physx.common.PxTolerancesScale;
import physx.common.PxVec3;
import physx.physics.PxPhysics;
import physx.physics.PxScene;
import physx.physics.PxSceneDesc;

public class PhysicsScene {
    private final PxScene scene;

    public PhysicsScene(PxPhysics physics, PxTolerancesScale tolerances) {
        final PxVec3 gravity = new PxVec3(0, -9.81f, 0);
        final PxSceneDesc descriptor = new PxSceneDesc(tolerances);

        descriptor.setGravity(gravity);
        descriptor.setCpuDispatcher(PxTopLevelFunctions.DefaultCpuDispatcherCreate(2));
        descriptor.setFilterShader(PxTopLevelFunctions.DefaultFilterShader());

        scene = physics.createScene(descriptor);
        gravity.destroy();
    }

    @Override
    protected void finalize() {
        scene.release();
    }

    public void setGravity(Float3 vec) {
        final PxVec3 gravity = new PxVec3(vec.x, vec.y, vec.z);
        scene.setGravity(gravity);
        gravity.destroy();
    }

    public Float3 getGravity() {
        final PxVec3 gravity = scene.getGravity();
        final Float3 result = new Float3(gravity.getX(), gravity.getY(), gravity.getZ());
        gravity.destroy();
        return result;
    }
}
