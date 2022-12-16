package gui.section

import editor.Editor
import gui.abs.GUISection
import imgui.ImGui
import imgui.extension.imguizmo.flag.Mode
import imgui.extension.imguizmo.flag.Operation
import imgui.flag.ImGuiStyleVar
import imgui.flag.ImGuiTableFlags
import imgui.flag.ImGuiWindowFlags
import scene.Scene

class GUIControlPanel(editor: Editor) : GUISection(editor) {
    private fun onGameStart() {
        editor.scene.toFile(SAVED_PLAY_PATH)
        editor.scriptEngine.callStarts(editor.scene)
        editor.timer.reset()
    }

    private fun onGameEnd() {
        editor.scene = Scene.fromFile(SAVED_PLAY_PATH, editor)!!
    }

    override fun renderGUI() {
        ImGui.pushStyleVar(ImGuiStyleVar.WindowPadding, 1f, 1f)
        if (ImGui.begin("Control Panel", ImGuiWindowFlags.NoScrollbar)) {
            if (ImGui.beginTable("##ControlTable", 3, ImGuiTableFlags.SizingStretchSame or ImGuiTableFlags.Borders, -1f, -1f)) {
                ImGui.tableNextRow()
                ImGui.tableNextColumn()
                if (ImGui.checkbox("Playing", editor.data.gameRunning)) {
                    if (!editor.data.gameRunning) {
                        onGameStart()
                    } else {
                        onGameEnd()
                    }
                    editor.data.gameRunning = !editor.data.gameRunning
                }
                ImGui.tableNextColumn()
                if (ImGui.checkbox("Wireframe", editor.data.wireframeState)) {
                    editor.data.wireframeState = !editor.data.wireframeState
                }
                ImGui.sameLine()
                if (ImGui.checkbox("Grid", editor.data.renderGrid)) {
                    editor.data.renderGrid = !editor.data.renderGrid
                }
                ImGui.tableNextColumn()
                if (ImGui.checkbox("Local", editor.data.gizmoMode == Mode.LOCAL)) {
                    editor.data.gizmoMode = if (editor.data.gizmoMode != Mode.LOCAL) Mode.LOCAL else Mode.WORLD
                }
                ImGui.sameLine()
                if (ImGui.checkbox("Scale", editor.data.gizmoOperation == Operation.SCALE)) {
                    editor.data.gizmoOperation = if (editor.data.gizmoOperation != Operation.SCALE) Operation.SCALE else 0
                }
                ImGui.sameLine()
                if (ImGui.checkbox("Rotation", editor.data.gizmoOperation == Operation.ROTATE)) {
                    editor.data.gizmoOperation = if (editor.data.gizmoOperation != Operation.ROTATE) Operation.ROTATE else 0
                }
                ImGui.sameLine()
                if (ImGui.checkbox("Translation", editor.data.gizmoOperation == Operation.TRANSLATE)) {
                    editor.data.gizmoOperation = if (editor.data.gizmoOperation != Operation.TRANSLATE) Operation.TRANSLATE else 0
                }
                ImGui.endTable()
            }
        }
        ImGui.end()
        ImGui.popStyleVar()
    }

    companion object {
        private const val SAVED_PLAY_PATH = "./temp/play.titian"
    }
}