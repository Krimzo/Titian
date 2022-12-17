package script.abs

import camera.abs.Camera
import editor.Editor
import entity.Entity
import light.AmbientLight
import light.DirectionalLight
import logging.LogInfo
import logging.LogType
import math.Int2

abstract class Scriptable(private val self: Entity) {
    abstract fun start()

    abstract fun update()

    fun log(obj: Any) {
        val sender = self.name + " [" + this.javaClass.name + "]"
        editor?.logger?.log(LogInfo(LogType.SCRIPT, sender, obj))
    }

    val windowPosition: Int2?
        get() = editor?.window?.position

    val windowSize: Int2?
        get() = editor?.window?.size

    fun setVSync(enabled: Boolean) {
        editor?.window?.setVSync(enabled)
    }

    val mousePosition: Int2?
        get() = editor?.window?.mousePosition

    val deltaT: Float?
        get() = editor?.timer?.getDeltaT()

    val elapsedT: Float?
        get() = editor?.timer?.getElapsedT()

    val sceneCamera: Camera?
        get() = editor?.scene?.selected?.camera

    val sceneAmbientLight: AmbientLight?
        get() = editor?.scene?.selected?.ambientLight

    val sceneDirectionalLight: DirectionalLight?
        get() = editor?.scene?.selected?.directionalLight

    companion object {
        private var editor: Editor? = null

        fun setEditor(editor: Editor) {
            Companion.editor = editor
        }
    }
}