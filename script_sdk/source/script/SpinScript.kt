package script

import sdk.engine.EngineScript
import sdk.engine.EngineState

class SpinScript(state: EngineState) : EngineScript(state) {
    override fun start() {
        println("Spin script started!")
    }

    override fun update() {
    }
}
