package test

import script.SpinScript
import sdk.engine.EngineState

fun main() {
    val state = EngineState(0f, 0f)
    val script = SpinScript(state)

    script.start()
    for (i in 0 until 5) {
        script.update()
    }
}
