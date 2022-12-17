package gui

import gui.abs.GUIRenderable
import gui.helper.GUIFonts
import gui.helper.GUIStyle
import imgui.ImGui
import imgui.extension.imguizmo.ImGuizmo
import imgui.flag.ImGuiConfigFlags
import imgui.flag.ImGuiDockNodeFlags
import imgui.gl3.ImGuiImplGl3
import imgui.glfw.ImGuiImplGlfw
import window.Window

class GUIRenderer(window: Window) : ArrayList<GUIRenderable>() {
    private val implGlfw: ImGuiImplGlfw = ImGuiImplGlfw()
    private val implGl3: ImGuiImplGl3 = ImGuiImplGl3()
    val fonts: GUIFontData
    val textures: GUITextureData

    init {
        ImGui.createContext()
        ImGui.getIO().configFlags = ImGuiConfigFlags.DockingEnable
        GUIStyle.reloadStyle()
        GUIFonts.clearGlobalAtlas()
        fonts = GUIFontData()
        implGlfw.init(window.window, true)
        implGl3.init("#version 330")
        textures = GUITextureData(window.context)
    }

    fun destroy() {
        implGl3.dispose()
        implGlfw.dispose()
        ImGui.destroyContext()
    }

    fun render() {
        implGlfw.newFrame()
        ImGui.newFrame()
        ImGuizmo.beginFrame()

        ImGui.dockSpaceOverViewport(ImGui.getMainViewport(), ImGuiDockNodeFlags.PassthruCentralNode)

        for (obj in this) {
            obj.renderGUI()
        }

        ImGui.render()
        implGl3.renderDrawData(ImGui.getDrawData())
    }
}