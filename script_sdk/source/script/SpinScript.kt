package script

import sdk.engine.Engine
import sdk.engine.Engine.log
import sdk.engine.Script
import sdk.render.scene.Entity

class SpinScript : Script() {
    override fun start() {
        log("Spin script started!")
    }

    override fun update() {
        val entityName = "Monke224"
        val entity: Entity = Engine.getEntity(entityName) ?: return

        log(entity.rotation)

        Engine.setEntity(entityName, entity)
    }
}
