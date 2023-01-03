package gui.section.script

import editor.Editor
import gui.abs.GUISection
import gui.helper.GUIDragDrop
import imgui.ImGui
import imgui.extension.texteditor.TextEditor
import imgui.extension.texteditor.TextEditorLanguageDefinition
import imgui.flag.ImGuiStyleVar
import imgui.flag.ImGuiWindowFlags
import utility.helper.FileHelper
import window.input.Input
import window.input.Key

class GUIScriptEditor(editor: Editor) : GUISection(editor) {
    private var textEditor: TextEditor = TextEditor()
    private var darkPalette: IntArray? = null
    private var filepath: String? = null
    private var lastMenuTextSize = 0f

    init {
        val languageDefinition = TextEditorLanguageDefinition.cPlusPlus()
        languageDefinition.setName("Java")
        languageDefinition.setKeywords(LanguageData.keywords)
        val identifiers: HashMap<String, String> = HashMap()
        for (iden in LanguageData.identifiers) {
            identifiers[iden] = ""
        }

        languageDefinition.setIdentifiers(identifiers)
        textEditor.setLanguageDefinition(languageDefinition)
        darkPalette = ColorPalette.getCustomDark(textEditor)
        textEditor.palette = darkPalette
        textEditor.tabSize = 4
        textEditor.setShowWhitespaces(false)
        textEditor.setColorizerEnable(true)
    }

    private fun saveFileText() {
        filepath?.let {
            FileHelper.writeString(it, textEditor.text.replace("\t".toRegex(), " ".repeat(4)))
        }
    }

    private fun loadFileText() {
        filepath?.let {
            textEditor.text = FileHelper.readString(it)
        }
    }

    private fun handleInput() {
        if (Input.isCtrlDown) {
            if (Input.isKeyPressed(Key.S)) {
                saveFileText()
            }
            if (Input.isKeyPressed(Key.R)) {
                loadFileText()
            }
        }
    }

    private fun renderMenu() {
        ImGui.pushStyleVar(ImGuiStyleVar.WindowPadding, 10f, 10f)
        if (ImGui.beginMenuBar()) {
            if (ImGui.beginMenu("File")) {
                if (ImGui.menuItem("Save", "Ctrl-S")) {
                    saveFileText()
                }
                if (ImGui.menuItem("Reload")) {
                    loadFileText()
                }
                if (ImGui.menuItem("Close")) {
                    filepath = null
                    textEditor.text = ""
                }
                ImGui.endMenu()
            }

            if (ImGui.beginMenu("Edit")) {
                if (ImGui.menuItem("Undo", "Ctrl-Z", false, textEditor.canUndo())) {
                    textEditor.undo(1)
                }
                if (ImGui.menuItem("Redo", "Ctrl-Y", false, textEditor.canRedo())) {
                    textEditor.redo(1)
                }
                ImGui.separator()
                if (ImGui.menuItem("Copy", "Ctrl-C", false, textEditor.hasSelection())) {
                    textEditor.copy()
                }
                if (ImGui.menuItem("Cut", "Ctrl-X", false, textEditor.hasSelection())) {
                    textEditor.cut()
                }
                if (ImGui.menuItem("Paste", "Ctrl-V", false, ImGui.getClipboardText() != null)) {
                    textEditor.paste()
                }
                ImGui.endMenu()
            }

            if (ImGui.beginMenu("View")) {
                if (ImGui.menuItem("Dark palette")) {
                    textEditor.palette = darkPalette
                }
                if (ImGui.menuItem("Light palette")) {
                    textEditor.palette = textEditor.lightPalette
                }
                ImGui.endMenu()
            }

            ImGui.setCursorPosX(ImGui.getWindowWidth() * 0.5f - lastMenuTextSize * 0.5f)
            ImGui.text(filepath ?: "Unknown")
            lastMenuTextSize = ImGui.getItemRectSizeX()
            ImGui.endMenuBar()
        }
        ImGui.popStyleVar()
    }

    private fun renderTextEditor() {
        ImGui.pushFont(editor.guiRenderer.fonts.scriptEditorFont)
        textEditor.render("Script Editor")
        GUIDragDrop.getData("ScriptFile") { filepath: Any ->
            if (filepath is String) {
                this.filepath = filepath
                loadFileText()
            }
        }
        ImGui.popFont()
    }

    override fun renderGUI() {
        ImGui.pushStyleVar(ImGuiStyleVar.WindowPadding, 1f, 1f)
        if (ImGui.begin("Script Editor##Window", ImGuiWindowFlags.MenuBar)) {
            handleInput()
            renderMenu()
            renderTextEditor()
        }
        ImGui.end()
        ImGui.popStyleVar()
    }
}