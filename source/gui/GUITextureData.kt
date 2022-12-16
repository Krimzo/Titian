package gui

import glparts.Texture
import window.GLContext

class GUITextureData(context: GLContext?) {
    val folderIcon: Texture
    val emptyFolderIcon: Texture
    val fileIcon: Texture
    val imageFileIcon: Texture
    val meshFileIcon: Texture
    val scriptFileIcon: Texture
    val codeFileIcon: Texture
    val sceneFileIcon: Texture

    init {
        folderIcon = Texture(null, null, context, "resource/textures/explorer/folder.png", false)
        emptyFolderIcon = Texture(null, null, context, "resource/textures/explorer/folder_empty.png", false)
        fileIcon = Texture(null, null, context, "resource/textures/explorer/file.png", false)
        imageFileIcon = Texture(null, null, context, "resource/textures/explorer/image.png", false)
        meshFileIcon = Texture(null, null, context, "resource/textures/explorer/mesh.png", false)
        scriptFileIcon = Texture(null, null, context, "resource/textures/explorer/script.png", false)
        codeFileIcon = Texture(null, null, context, "resource/textures/explorer/code.png", false)
        sceneFileIcon = Texture(null, null, context, "resource/textures/explorer/scene.png", false)
    }
}