package gui.section;

import editor.Editor;
import glparts.Texture;
import gui.abs.GUISection;
import imgui.ImGui;
import imgui.ImVec4;
import imgui.flag.ImGuiCol;
import imgui.flag.ImGuiInputTextFlags;
import imgui.flag.ImGuiPopupFlags;
import imgui.flag.ImGuiWindowFlags;
import imgui.type.ImString;
import script.Script;
import utility.Files;

import java.awt.*;
import java.io.File;
import java.nio.file.Paths;

public final class GUIExplorer extends GUISection {
    private File currentPath = new File(Paths.get("").toUri());
    private float buttonSize = 0;
    private ImString scriptNameInput = null;

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

    private String getFileType(File file) {
        return switch (Files.getExtension(file.toString())) {
            case "obj" -> "MeshFile";
            case "jpg", "png", "bmp" -> "ImageFile";
            case "c", "cpp", "h", "glsl" -> "CodeFile";
            case "java" -> "ScriptFile";
            default -> "File";
        };
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
        Texture fileIcon = getFileIcon(file);

        ImGui.pushID(file.toString());
        if (ImGui.imageButton(fileIcon.getBuffer(), buttonSize, buttonSize)) {
            try {
                Desktop.getDesktop().open(file);
            }
            catch (Exception ignored) {
                System.out.println("File \"" + file + "\" open with default error");
            }
        }
        ImGui.popID();

        if (ImGui.beginDragDropSource()) {
            ImGui.setDragDropPayload(getFileType(file), file.getAbsolutePath());
            ImGui.image(fileIcon.getBuffer(), 50, 50);
            ImGui.endDragDropSource();
        }

        ImGui.textWrapped(file.getName());
        ImGui.nextColumn();
    }

    @Override
    public void renderGUI() {
        if (ImGui.begin("Explorer", ImGuiWindowFlags.NoScrollbar)) {
            buttonSize = 0.75f * ImGui.getWindowContentRegionMaxX() / columnCount;

            ImGui.bulletText(currentPath.getAbsolutePath());
            ImGui.separator();

            ImVec4 color = ImGui.getStyle().getColor(ImGuiCol.Button);
            ImGui.pushStyleColor(ImGuiCol.Button, color.x, color.y, color.z, 0.0f);

            color = ImGui.getStyle().getColor(ImGuiCol.ButtonHovered);
            ImGui.pushStyleColor(ImGuiCol.ButtonHovered, color.x, color.y, color.z, 0.25f);

            color = ImGui.getStyle().getColor(ImGuiCol.ButtonActive);
            ImGui.pushStyleColor(ImGuiCol.ButtonActive, color.x, color.y, color.z, 0.5f);

            ImGui.columns(columnCount, "ExplorerColumns", false);

            renderParent();

            File[] folders = Files.listFolders(currentPath.toString());
            if (folders != null) {
                for (File folder : folders) {
                    renderFolder(folder);
                }
            }

            File[] files = Files.listFiles(currentPath.toString());
            if (files != null) {
                for (File file : files) {
                    renderFile(file);
                }
            }

            ImGui.popStyleColor(3);

            if (ImGui.beginPopupContextWindow(ImGuiPopupFlags.MouseButtonRight | ImGuiPopupFlags.NoOpenOverItems)) {
                if (scriptNameInput == null) {
                    if (ImGui.button("New Script")) {
                        scriptNameInput = new ImString();
                    }
                }
                else if (ImGui.inputText("Script Name", scriptNameInput, ImGuiInputTextFlags.EnterReturnsTrue)) {
                    String scriptName = scriptNameInput.toString();
                    Files.writeString(currentPath + "/" + scriptName + ".java", Script.getTemplate(scriptName));
                    scriptNameInput = null;

                    ImGui.closeCurrentPopup();
                }

                ImGui.endPopup();
            }
        }
        ImGui.end();
    }
}
