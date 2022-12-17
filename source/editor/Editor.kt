package editor

import camera.PerspectiveCamera
import glparts.abs.GLObject
import gui.GUIRenderer
import gui.section.*
import gui.section.script.GUIScriptEditor
import logging.Logger
import math.Int2
import named.NameHolder
import physics.PhysicsEngine
import renderer.EditorRenderer
import renderer.GameRenderer
import scene.Scene
import script.ScriptEngine
import utility.Timer
import window.Window

class Editor {
    val data: EditorData
    val window: Window = Window(Int2(1600, 900), "Titian", true)
    val timer: Timer = Timer()
    val camera: PerspectiveCamera = PerspectiveCamera(NameHolder(), "Editor Camera", this)
    val editorRenderer: EditorRenderer = EditorRenderer(window.context, window.size)
    val gameRenderer: GameRenderer = GameRenderer(window.context, window.size)
    val guiRenderer: GUIRenderer = GUIRenderer(window)
    val physicsEngine: PhysicsEngine = PhysicsEngine()
    val logger: Logger = Logger()
    val scriptEngine: ScriptEngine = ScriptEngine()
    var scene: Scene = Scene()

    init {
        window.setIcon("resource/textures/titian.png")
        window.context.setDepthTest(true)
        window.setVSync(true)

        data = EditorData(window.context, this)

        guiRenderer.add(GUIMainMenu(this))
        guiRenderer.add(GUIScene(this))
        guiRenderer.add(GUIControlPanel(this))
        guiRenderer.add(GUIMeshEditor(this))
        guiRenderer.add(GUIMaterialEditor(this))
        guiRenderer.add(GUIScriptEditor(this))
        guiRenderer.add(GUIGameView(this))
        guiRenderer.add(GUIViewport(this))
        guiRenderer.add(GUILogView(this))
        guiRenderer.add(GUIExplorer(this))
        guiRenderer.add(GUIProfiling(this))
        guiRenderer.add(GUIProperties(this))
    }

    fun destroy() {
        guiRenderer.destroy()
        window.destroy()
    }

    fun setup(setupCallback: (Editor) -> Unit) {
        setupCallback(this)
        window.maximize()
        timer.reset()
    }

    fun update() {
        timer.updateDeltaT()

        if (data.gameRunning) {
            physicsEngine.update(scene, timer.getDeltaT())
            scriptEngine.callUpdates(scene)
        }

        window.context.clear(false)
        window.context.setViewport(window.size)

        guiRenderer.render()
        window.swapBuffers()

        GLObject.cleanup()
    }
}