package gui

import glparts.Texture
import named.NameHolder
import window.GLContext

class GUITextureData(context: GLContext) {
    val folderIcon: Texture
    val emptyFolderIcon: Texture
    val fileIcon: Texture
    val imageFileIcon: Texture
    val meshFileIcon: Texture
    val scriptFileIcon: Texture
    val codeFileIcon: Texture
    val sceneFileIcon: Texture

    init {
        folderIcon = Texture(NameHolder(), "Texture", context, "resource/textures/explorer/folder.png", false)
        emptyFolderIcon = Texture(NameHolder(), "Texture", context, "resource/textures/explorer/folder_empty.png", false)
        fileIcon = Texture(NameHolder(), "Texture", context, "resource/textures/explorer/file.png", false)
        imageFileIcon = Texture(NameHolder(), "Texture", context, "resource/textures/explorer/image.png", false)
        meshFileIcon = Texture(NameHolder(), "Texture", context, "resource/textures/explorer/mesh.png", false)
        scriptFileIcon = Texture(NameHolder(), "Texture", context, "resource/textures/explorer/script.png", false)
        codeFileIcon = Texture(NameHolder(), "Texture", context, "resource/textures/explorer/code.png", false)
        sceneFileIcon = Texture(NameHolder(), "Texture", context, "resource/textures/explorer/scene.png", false)
    }
}