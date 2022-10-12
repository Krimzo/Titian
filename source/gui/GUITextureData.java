package gui;

import glparts.Texture;
import glparts.abs.Disposable;
import glparts.abs.GLContext;

public class GUITextureData implements Disposable {
    public final Texture playIcon;
    public final Texture stopIcon;
    public final Texture wireIcon;
    public final Texture solidIcon;

    public final Texture folderIcon;
    public final Texture emptyFolderIcon;
    public final Texture fileIcon;
    public final Texture imageFileIcon;
    public final Texture meshFileIcon;
    public final Texture scriptFileIcon;
    public final Texture codeFileIcon;
    public final Texture sceneFileIcon;

    public GUITextureData(GLContext context) {
        playIcon = new Texture(null, null, context, "resource/textures/control/play.png", false);
        stopIcon = new Texture(null, null, context, "resource/textures/control/stop.png", false);
        wireIcon = new Texture(null, null, context, "resource/textures/control/wire.png", false);
        solidIcon =  new Texture(null, null, context, "resource/textures/control/solid.png", false);

        folderIcon = new Texture(null, null, context, "resource/textures/explorer/folder.png", false);
        emptyFolderIcon = new Texture(null, null, context, "resource/textures/explorer/folder_empty.png", false);
        fileIcon = new Texture(null, null, context, "resource/textures/explorer/file.png", false);
        imageFileIcon = new Texture(null, null, context, "resource/textures/explorer/image.png", false);
        meshFileIcon = new Texture(null, null, context, "resource/textures/explorer/mesh.png", false);
        scriptFileIcon = new Texture(null, null, context, "resource/textures/explorer/script.png", false);
        codeFileIcon = new Texture(null, null, context, "resource/textures/explorer/code.png", false);
        sceneFileIcon = new Texture(null, null, context, "resource/textures/explorer/scene.png", false);
    }

    @Override
    public void dispose() {
        playIcon.dispose();
        stopIcon.dispose();
        wireIcon.dispose();
        solidIcon.dispose();

        folderIcon.dispose();
        emptyFolderIcon.dispose();
        fileIcon.dispose();
        imageFileIcon.dispose();
        meshFileIcon.dispose();
        scriptFileIcon.dispose();
        codeFileIcon.dispose();
        sceneFileIcon.dispose();
    }
}
