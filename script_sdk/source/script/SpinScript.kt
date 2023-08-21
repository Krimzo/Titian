package script

import sdk.engine.Engine
import sdk.engine.Script
import sdk.render.scene.Entity

class SpinScript : Script() {
    override fun start() {
        println("Spin script started!")
    }

    override fun update() {
        val entity: Entity = Engine.readEntity("Monke224") ?: return

        println(entity.renderScale)

        Engine.writeEntity("monke1", entity)
    }
}
