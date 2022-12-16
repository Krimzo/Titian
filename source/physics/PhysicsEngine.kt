package physics

import physx.PxTopLevelFunctions
import physx.common.PxDefaultErrorCallback
import physx.common.PxFoundation
import physx.common.PxTolerancesScale
import physx.extensions.PxDefaultAllocator
import physx.physics.PxPhysics
import scene.Scene

class PhysicsEngine {
    private val allocator: PxDefaultAllocator
    private val errorCallback: PxDefaultErrorCallback
    private val foundation: PxFoundation
    private val tolerances: PxTolerancesScale
    private val physics: PxPhysics

    init {
        val version = PxTopLevelFunctions.getPHYSICS_VERSION()
        allocator = PxDefaultAllocator()
        errorCallback = PxDefaultErrorCallback()
        foundation = PxTopLevelFunctions.CreateFoundation(version, allocator, errorCallback)
        tolerances = PxTolerancesScale()
        physics = PxTopLevelFunctions.CreatePhysics(version, foundation, tolerances)
    }

    protected fun finalize() {
        physics.release()
        tolerances.destroy()
        foundation.release()
        errorCallback.destroy()
        allocator.destroy()
    }

    fun update(scene: Scene?, deltaT: Float) {
        for (physical in scene!!) {
            physical?.updatePhysics(deltaT)
        }
    }
}