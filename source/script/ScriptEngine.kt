package script

import scene.Scene

class ScriptEngine {
    fun callStarts(scene: Scene) {
        for (entity in scene) {
            entity.callScriptStarts()
        }
    }

    fun callUpdates(scene: Scene) {
        for (entity in scene) {
            entity.callScriptUpdates()
        }
    }
}