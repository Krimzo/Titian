package gui;

import glparts.Texture;
import window.GLContext;

public final class GUITextureData {
    public final Texture folderIcon;
    public final Texture emptyFolderIcon;
    public final Texture fileIcon;
    public final Texture imageFileIcon;
    public final Texture meshFileIcon;
    public final Texture scriptFileIcon;
    public final Texture codeFileIcon;
    public final Texture sceneFileIcon;

    public GUITextureData(GLContext context) {
        folderIcon = new Texture(null, null, context, "resource/textures/explorer/folder.png", false);
        emptyFolderIcon = new Texture(null, null, context, "resource/textures/explorer/folder_empty.png", false);
        fileIcon = new Texture(null, null, context, "resource/textures/explorer/file.png", false);
        imageFileIcon = new Texture(null, null, context, "resource/textures/explorer/image.png", false);
        meshFileIcon = new Texture(null, null, context, "resource/textures/explorer/mesh.png", false);
        scriptFileIcon = new Texture(null, null, context, "resource/textures/explorer/script.png", false);
        codeFileIcon = new Texture(null, null, context, "resource/textures/explorer/code.png", false);
        sceneFileIcon = new Texture(null, null, context, "resource/textures/explorer/scene.png", false);
    }
}
