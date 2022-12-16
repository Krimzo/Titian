package gui.section

import editor.Editor
import gui.abs.GUISection
import gui.helper.GUIDisplay
import gui.helper.GUIEdit
import gui.helper.GUIStyle
import gui.helper.GUITextInput
import imgui.ImGui
import imgui.ImVec4
import imgui.flag.ImGuiStyleVar
import math.Float2
import math.Int2
import scene.Scene
import utility.helper.FileHelper
import window.input.Input
import window.input.Mouse

class GUIMainMenu(editor: Editor) : GUISection(editor) {
    private var textInput: GUITextInput? = null
    private fun windowControl() {
        val rect = ImVec4(ImGui.getWindowPosX(), ImGui.getWindowPosY(), 0f, 0f)
        rect.z = rect.x + ImGui.getWindowWidth()
        rect.w = rect.y + ImGui.getWindowHeight()
        if (ImGui.isMouseHoveringRect(rect.x, rect.y, rect.z, rect.w)) {
            var maximized = editor.window.isMaximized
            if (ImGui.isMouseDoubleClicked(Mouse.Left)) {
                if (maximized) {
                    editor.window.restore()
                } else {
                    editor.window.maximize()
                }
                maximized = !maximized
            }
            if (!maximized && Input.isMouseDown(Mouse.Left)) {
                val pos = editor.window.position
                val drag = ImGui.getMouseDragDelta()
                editor.window.position = Int2(pos!!.x + drag.x.toInt(), pos.y + drag.y.toInt())
            }
        }
    }

    private fun fileMenu() {
        if (ImGui.beginMenu("File")) {
            if (ImGui.menuItem("New scene")) {
                editor.scene = Scene()
            }
            if (ImGui.menuItem("Save scene")) {
                textInput = GUITextInput(FileHelper.defaultPath() + FileHelper.separator) { path: String ->
                    val fullPath = "$path.titian"
                    if (editor.scene.toFile(fullPath)) {
                        println("Scene \"$fullPath\" saved!")
                    }
                    textInput = null
                }
            }
            if (ImGui.menuItem("Exit")) {
                editor.window.close()
            }
            ImGui.endMenu()
        }
    }

    private fun editMenu() {
        if (ImGui.beginMenu("Edit")) {
            if (ImGui.menuItem("Reload scripts")) {
                for (entity in editor.scene) {
                    entity?.components?.script?.reload()
                }
            }
            ImGui.endMenu()
        }
    }

    private fun viewMenu() {
        if (ImGui.beginMenu("View")) {
            if (ImGui.beginMenu("Colors")) {
                GUIEdit.editColor3("Black", GUIStyle.black)
                GUIEdit.editColor3("Dark", GUIStyle.dark)
                GUIEdit.editColor3("Normal", GUIStyle.normal)
                GUIEdit.editColor3("Light", GUIStyle.light)
                GUIEdit.editColor3("Special", GUIStyle.special)
                if (ImGui.button("Reload style", -1f, 0f)) {
                    GUIStyle.reloadStyle()
                }
                if (ImGui.button("Load defaults", -1f, 0f)) {
                    GUIStyle.loadDefaults()
                }
                ImGui.endMenu()
            }
            ImGui.endMenu()
        }
    }

    private fun renderMenu() {
        if (ImGui.beginMenu("Render")) {
            val size = Float2(1280f, 720f)
            if (ImGui.beginMenu("Depth Texture")) {
                GUIDisplay.texture(editor.editorRenderer.renderBuffer.getDepthMap(), size)
                ImGui.endMenu()
            }
            if (ImGui.beginMenu("Index Texture")) {
                GUIDisplay.texture(editor.editorRenderer.indexBuffer.getColorMap(), size)
                ImGui.endMenu()
            }
            ImGui.endMenu()
        }
    }

    private var barButtonSizes = 0f
    private fun barButtons() {
        ImGui.setCursorPosX(ImGui.getWindowPosX() + ImGui.getWindowWidth() - barButtonSizes)
        barButtonSizes = 0f
        if (ImGui.menuItem("  --  ")) {
            editor.window.minimize()
        }
        barButtonSizes += ImGui.getItemRectSizeX()
        if (ImGui.menuItem("  O  ")) {
            if (editor.window.isMaximized) {
                editor.window.restore()
            } else {
                editor.window.maximize()
            }
        }
        barButtonSizes += ImGui.getItemRectSizeX()
        if (ImGui.menuItem("  X  ")) {
            editor.window.close()
        }
        barButtonSizes += ImGui.getItemRectSizeX()
    }

    override fun renderGUI() {
        ImGui.pushStyleVar(ImGuiStyleVar.FramePadding, 0f, 8f)
        if (ImGui.beginMainMenuBar()) {
            windowControl()
            ImGui.text("TITIAN")
            fileMenu()
            editMenu()
            viewMenu()
            renderMenu()
            barButtons()
            textInput?.update()
        }
        ImGui.endMainMenuBar()
        ImGui.popStyleVar()
    }
}