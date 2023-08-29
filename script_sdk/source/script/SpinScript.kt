package script

import sdk.engine.Engine
import sdk.engine.Script
import sdk.math.vector.Float3
import kotlin.math.sin

class SpinScript : Script() {
    private var entityName = "Monke224"
    private var originalPosition = Float3()

    override fun start() {
        Engine.readEntity(entityName)?.position?.let {
            originalPosition = it
        }
    }
    
    override fun update() {
        Engine.readEntity(entityName)?.let { entity ->
            val timedValue = sin(Engine.elapsedTime) * 5f
            entity.position.z = originalPosition.z + timedValue

            Engine.writeEntity(entityName, entity)
        }
    }
}
