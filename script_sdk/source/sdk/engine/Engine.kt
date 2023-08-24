package sdk.engine

import sdk.render.scene.Entity

object Engine {
    // Time
    var elapsedTime: Float = 0f
        private set
    var deltaTime: Float = 0f
        private set

    // System
    private external fun logExt(data: String)

    // Entity
    external fun getEntity(name: String): Entity?
    external fun setEntity(name: String, entity: Entity)

    // Helper
    fun log(any: Any?) {
        logExt(any.toString())
    }
}
