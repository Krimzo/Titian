package gui.section

import editor.Editor
import glparts.Texture
import gui.abs.GUISection
import gui.helper.GUIDragDrop
import gui.helper.GUIPopup
import gui.helper.GUITextInput
import imgui.ImGui
import imgui.flag.ImGuiCol
import imgui.flag.ImGuiWindowFlags
import script.Script
import utility.helper.FileHelper
import java.io.File

class GUIExplorer(editor: Editor) : GUISection(editor) {
    private var textInput: GUITextInput? = null
    private var currentPath: File = File(FileHelper.defaultPath())
    private var buttonSize: Float = 0f
    private var columnCount: Int = 12

    private fun getFolderIcon(folder: File): Texture {
        return if (FileHelper.isEmpty(folder.toString())) {
            editor.guiRenderer.textures.emptyFolderIcon
        }
        else {
            editor.guiRenderer.textures.folderIcon
        }
    }

    private fun getFileType(file: File): String {
        return when (FileHelper.getExtension(file.toString())) {
            "obj" -> "MeshFile"
            "jpg", "png", "bmp" -> "ImageFile"
            "c", "cpp", "h", "glsl" -> "CodeFile"
            "java" -> "ScriptFile"
            "titian" -> "SceneFile"
            else -> "File"
        }
    }

    private fun getFileIcon(file: File): Texture {
        return when (getFileType(file)) {
            "MeshFile" -> editor.guiRenderer.textures.meshFileIcon
            "ImageFile" -> editor.guiRenderer.textures.imageFileIcon
            "CodeFile" -> editor.guiRenderer.textures.codeFileIcon
            "ScriptFile" -> editor.guiRenderer.textures.scriptFileIcon
            "SceneFile" -> editor.guiRenderer.textures.sceneFileIcon
            else -> editor.guiRenderer.textures.fileIcon
        }
    }

    private fun renderFolder(folder: File, parent: Boolean) {
        val folderIcon = getFolderIcon(folder)
        ImGui.pushID(folder.toString())
        if (ImGui.imageButton(folderIcon.buffer, buttonSize, buttonSize)) {
            currentPath = folder
        }
        ImGui.popID()

        GUIDragDrop.setData("Folder", folder.absolutePath, folderIcon)
        GUIPopup.itemPopup("EditExplorerFolder$folder") {
            if (ImGui.button("Rename")) {
                textInput = GUITextInput(folder.name) { name: String ->
                    FileHelper.rename(folder.absolutePath, name)
                    textInput = null
                }
                GUIPopup.close()
            }
            if (FileHelper.isEmpty(folder.absolutePath) && ImGui.button("Delete", -1f, 0f)) {
                FileHelper.delete(folder.absolutePath)
                GUIPopup.close()
            }
        }

        val callback = { path: Any? ->
            FileHelper.move(path as String, folder.absolutePath); Unit
        }
        GUIDragDrop.getData("Folder", callback)
        GUIDragDrop.getData("File", callback)
        GUIDragDrop.getData("MeshFile", callback)
        GUIDragDrop.getData("ImageFile", callback)
        GUIDragDrop.getData("CodeFile", callback)
        GUIDragDrop.getData("ScriptFile", callback)
        GUIDragDrop.getData("SceneFile", callback)
        ImGui.text(if (!parent) folder.name else "..")
        ImGui.nextColumn()
    }

    private fun renderFile(file: File) {
        val fileIcon = getFileIcon(file)
        ImGui.pushID(file.toString())
        if (ImGui.imageButton(fileIcon.buffer, buttonSize, buttonSize)) {
            FileHelper.openDefault(file.absolutePath)
        }
        ImGui.popID()

        GUIDragDrop.setData(getFileType(file), file.absolutePath, fileIcon)
        GUIPopup.itemPopup("EditExplorerFile$file") {
            if (ImGui.button("Rename")) {
                textInput = GUITextInput(file.name) { name: String ->
                    FileHelper.rename(file.absolutePath, name)
                    textInput = null
                }
                GUIPopup.close()
            }
            if (ImGui.button("Delete", -1f, 0f)) {
                FileHelper.delete(file.absolutePath)
                GUIPopup.close()
            }
        }
        ImGui.text(file.name)
        ImGui.nextColumn()
    }

    override fun renderGUI() {
        if (ImGui.begin("Explorer", ImGuiWindowFlags.NoScrollbar)) {
            ImGui.bulletText(currentPath.absolutePath)
            ImGui.separator()
            var color = ImGui.getStyle().getColor(ImGuiCol.Button)

            ImGui.pushStyleColor(ImGuiCol.Button, color.x, color.y, color.z, 0.0f)
            color = ImGui.getStyle().getColor(ImGuiCol.ButtonHovered)
            ImGui.pushStyleColor(ImGuiCol.ButtonHovered, color.x, color.y, color.z, 0.25f)
            color = ImGui.getStyle().getColor(ImGuiCol.ButtonActive)
            ImGui.pushStyleColor(ImGuiCol.ButtonActive, color.x, color.y, color.z, 0.5f)

            ImGui.columns(columnCount, "ExplorerColumns", false)
            buttonSize = ImGui.calcItemWidth()
            currentPath.parentFile?.let {
                renderFolder(it, true)
            }

            val folders = FileHelper.listFolders(currentPath.toString())
            folders?.let {
                for (folder in it) {
                    renderFolder(folder, false)
                }
            }

            val files = FileHelper.listFiles(currentPath.toString())
            files?.let {
                for (file in it) {
                    renderFile(file)
                }
            }

            ImGui.popStyleColor(3)
            GUIPopup.windowPopup("ExplorerWindow") {
                if (ImGui.button("New File", -1f, 0f)) {
                    textInput = GUITextInput("") { name: String ->
                        FileHelper.createFile("$currentPath/$name")
                        textInput = null
                    }
                    GUIPopup.close()
                }

                if (ImGui.button("New Script", -1f, 0f)) {
                    textInput = GUITextInput("") { name: String ->
                        val path = "$currentPath/$name.java"
                        FileHelper.writeString(path, Script.getTemplate(name))
                        textInput = null
                    }
                    GUIPopup.close()
                }

                if (ImGui.button("New Folder")) {
                    textInput = GUITextInput("") { name: String ->
                        FileHelper.createFolder("$currentPath/$name")
                        textInput = null
                    }
                    GUIPopup.close()
                }
            }

            textInput?.update()
        }
        ImGui.end()
    }
}