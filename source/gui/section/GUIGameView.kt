package gui.section

import camera.PerspectiveCamera
import editor.Editor
import gui.abs.GUISection
import gui.helper.GUIDisplay
import imgui.ImGui
import imgui.flag.ImGuiStyleVar
import imgui.flag.ImGuiWindowFlags
import math.Int2
import named.NameHolder

class GUIGameView(editor: Editor) : GUISection(editor) {
    private var windowSize: Int2 = Int2()

    private fun updateWindowInfo() {
        val tabSize = ImGui.getWindowContentRegionMinY().toInt()
        windowSize = Int2(ImGui.getWindowWidth().toInt(), ImGui.getWindowHeight().toInt() - tabSize)
    }

    private fun renderScene() {
        val camera = editor.scene.selected.camera
        if (camera == null) {
            editor.gameRenderer.clear(PerspectiveCamera(NameHolder(), "Camera", editor))
            return
        }

        editor.gameRenderer.clear(camera)
        editor.window.context.setViewport(windowSize)
        editor.gameRenderer.resize(windowSize)

        if (editor.scene.selected.camera?.javaClass == PerspectiveCamera::class.java) {
            (editor.scene.selected.camera as PerspectiveCamera).updateAspect(windowSize)
        }
        editor.gameRenderer.renderScene(editor.scene, camera)
    }

    override fun renderGUI() {
        ImGui.pushStyleVar(ImGuiStyleVar.WindowPadding, 0f, 0f)
        if (ImGui.begin("Game View", ImGuiWindowFlags.NoScrollbar)) {
            updateWindowInfo()
            renderScene()
            GUIDisplay.texture(editor.gameRenderer.renderBuffer.getColorMap(), windowSize)
        }
        ImGui.end()
        ImGui.popStyleVar()
    }
}