package sdk.engine

import sdk.render.scene.Entity

object Engine {
    // Time
    var elapsedTime: Float = 0f
        private set
    var deltaTime: Float = 0f
        private set

    // External
    external fun logString(data: String)
    external fun readEntity(name: String): Entity?
    external fun writeEntity(name: String, entity: Entity)

    // Methods
    fun log(any: Any?) {
        logString(any.toString())
    }
}
