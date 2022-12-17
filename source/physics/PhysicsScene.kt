package physics

import math.Float3
import physx.PxTopLevelFunctions
import physx.common.PxTolerancesScale
import physx.common.PxVec3
import physx.physics.PxPhysics
import physx.physics.PxScene
import physx.physics.PxSceneDesc

class PhysicsScene(physics: PxPhysics, tolerances: PxTolerancesScale) {
    private val scene: PxScene

    init {
        val gravity = PxVec3(0f, -9.81f, 0f)
        val descriptor = PxSceneDesc(tolerances)

        descriptor.gravity = gravity
        descriptor.cpuDispatcher = PxTopLevelFunctions.DefaultCpuDispatcherCreate(2)
        descriptor.filterShader = PxTopLevelFunctions.DefaultFilterShader()

        scene = physics.createScene(descriptor)
        gravity.destroy()
    }

    protected fun finalize() {
        scene.release()
    }

    fun setGravity(vec: Float3) {
        val gravity = PxVec3(vec.x, vec.y, vec.z)
        scene.gravity = gravity
        gravity.destroy()
    }

    fun getGravity(): Float3 {
        val gravity = scene.gravity
        val result = Float3(gravity.x, gravity.y, gravity.z)
        gravity.destroy()
        return result
    }
}