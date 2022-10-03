package gui.section;

import editor.Editor;
import glparts.Texture;
import gui.abs.GUISection;
import imgui.ImGui;
import imgui.ImGuiStyle;
import imgui.ImVec4;
import imgui.flag.ImGuiCol;
import imgui.flag.ImGuiWindowFlags;
import utility.Files;

import java.awt.*;
import java.io.File;
import java.nio.file.Paths;

public final class GUIExplorer extends GUISection {
    private File currentPath = new File(Paths.get("").toUri());
    private float buttonSize = 0;

    public int columnCount = 12;

    public GUIExplorer(Editor editor) {
        super(editor);
    }

    private Texture getFolderIcon(File folder) {
        if (Files.isEmpty(folder.toString())) {
            return editor.guiRenderer.predefineTextures.get("EmptyFolderIcon");
        }
        return editor.guiRenderer.predefineTextures.get("FolderIcon");
    }

    private Texture getFileIcon(File file) {
        return editor.guiRenderer.predefineTextures.get(switch (Files.getExtension(file.toString())) {
            case "obj" -> "MeshFileIcon";
            case "jpg", "png", "bmp" -> "ImageFileIcon";
            case "c", "cpp", "h", "glsl" -> "CodeFileIcon";
            case "java" -> "ScriptFileIcon";
            default -> "FileIcon";
        });
    }

    private void renderParent() {
        File parent = currentPath.getParentFile();
        if (parent != null) {
            ImGui.pushID(parent.toString());
            if (ImGui.imageButton(getFolderIcon(parent).getBuffer(), buttonSize, buttonSize)) {
                currentPath = parent;
            }
            ImGui.popID();

            ImGui.textWrapped("..");
            ImGui.nextColumn();
        }
    }

    private void renderFolder(File folder) {
        ImGui.pushID(folder.toString());
        if (ImGui.imageButton(getFolderIcon(folder).getBuffer(), buttonSize, buttonSize)) {
            currentPath = folder;
        }
        ImGui.popID();

        ImGui.textWrapped(folder.getName());
        ImGui.nextColumn();
    }

    private void renderFile(File file) {
        ImGui.pushID(file.toString());
        if (ImGui.imageButton(getFileIcon(file).getBuffer(), buttonSize, buttonSize)) {
            try {
                Desktop.getDesktop().open(file);
            }
            catch (Exception e) {
                e.printStackTrace();
            }
        }
        ImGui.popID();

        ImGui.textWrapped(file.getName());
        ImGui.nextColumn();
    }

    @Override
    public void renderGUI() {
        if (ImGui.begin("Explorer", ImGuiWindowFlags.NoScrollbar)) {
            buttonSize = 0.75f * ImGui.getWindowContentRegionMaxX() / columnCount;

            ImGui.text(currentPath.getAbsolutePath());
            ImGui.separator();

            ImGuiStyle style = ImGui.getStyle();
            ImVec4 buttonColor = style.getColor(ImGuiCol.Button);
            ImVec4 buttonHoverColor = style.getColor(ImGuiCol.ButtonHovered);
            ImVec4 buttonActiveColor = style.getColor(ImGuiCol.ButtonActive);
            ImGui.pushStyleColor(ImGuiCol.Button, buttonColor.x, buttonColor.y, buttonColor.z, 0.0f);
            ImGui.pushStyleColor(ImGuiCol.ButtonHovered, buttonHoverColor.x, buttonHoverColor.y, buttonHoverColor.z, 0.25f);
            ImGui.pushStyleColor(ImGuiCol.ButtonActive, buttonActiveColor.x, buttonActiveColor.y, buttonActiveColor.z, 0.5f);

            ImGui.columns(columnCount, "ExplorerColumns", false);

            renderParent();

            for (File folder : Files.listFolders(currentPath.toString())) {
                renderFolder(folder);
            }

            for (File file : Files.listFiles(currentPath.toString())) {
                renderFile(file);
            }

            ImGui.popStyleColor(3);
        }
        ImGui.end();
    }
}
