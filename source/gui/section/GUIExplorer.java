package gui.section;

import callback.ObjectCallback;
import editor.Editor;
import glparts.Texture;
import gui.GUIDragDrop;
import gui.GUIPopup;
import gui.GUITextInput;
import gui.abs.GUISection;
import imgui.ImGui;
import imgui.ImVec4;
import imgui.flag.ImGuiCol;
import imgui.flag.ImGuiWindowFlags;
import script.Script;
import utility.Files;

import java.io.File;
import java.nio.file.Paths;

public final class GUIExplorer extends GUISection {
    private GUITextInput textInput = null;

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

    private String getFileType(File file) {
        return switch (Files.getExtension(file.toString())) {
            case "obj" -> "MeshFile";
            case "jpg", "png", "bmp" -> "ImageFile";
            case "c", "cpp", "h", "glsl" -> "CodeFile";
            case "java" -> "ScriptFile";
            case "titian" -> "SceneFile";
            default -> "File";
        };
    }

    private Texture getFileIcon(File file) {
        return editor.guiRenderer.predefineTextures.get(getFileType(file) + "Icon");
    }

    private void renderParent() {
        File parent = currentPath.getParentFile();
        if (parent != null) {
            renderFolder(parent, true);
        }
    }

    private void renderFolder(File folder, boolean parent) {
        ImGui.pushID(folder.toString());
        if (ImGui.imageButton(getFolderIcon(folder).getBuffer(), buttonSize, buttonSize)) {
            currentPath = folder;
        }
        ImGui.popID();

        GUIPopup.itemPopup("EditExplorerFolder" + folder, () -> {
            if (ImGui.button("Rename")) {
                textInput = new GUITextInput(folder.getAbsolutePath(), name -> {
                    Files.rename(folder.getAbsolutePath(), name);
                    textInput = null;
                });
                GUIPopup.close();
            }

            if (Files.isEmpty(folder.getAbsolutePath()) && ImGui.button("Delete", -1, 0)) {
                Files.delete(folder.getAbsolutePath());
                GUIPopup.close();
            }
        });

        GUIDragDrop.setData("Folder", folder.getAbsolutePath());

        ObjectCallback callback = path -> {
            Files.move((String) path, folder.getAbsolutePath());
        };
        GUIDragDrop.getData("Folder", callback);
        GUIDragDrop.getData("File", callback);
        GUIDragDrop.getData("MeshFile", callback);
        GUIDragDrop.getData("ImageFile", callback);
        GUIDragDrop.getData("CodeFile", callback);
        GUIDragDrop.getData("ScriptFile", callback);
        GUIDragDrop.getData("SceneFile", callback);

        ImGui.textWrapped(!parent ? folder.getName() : "..");
        ImGui.nextColumn();
    }

    private void renderFile(File file) {
        final Texture fileIcon = getFileIcon(file);

        ImGui.pushID(file.toString());
        if (ImGui.imageButton(fileIcon.getBuffer(), buttonSize, buttonSize)) {
            Files.openDefault(file.getAbsolutePath());
        }
        ImGui.popID();

        GUIPopup.itemPopup("EditExplorerFile" + file, () -> {
            if (ImGui.button("Rename")) {
                textInput = new GUITextInput(file.getName(), name -> {
                    Files.rename(file.getAbsolutePath(), name);
                    textInput = null;
                });
                GUIPopup.close();
            }

            if (ImGui.button("Delete", -1, 0)) {
                Files.delete(file.getAbsolutePath());
                GUIPopup.close();
            }
        });

        GUIDragDrop.setData(getFileType(file), file.getAbsolutePath(), fileIcon);

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
                    renderFolder(folder, false);
                }
            }

            File[] files = Files.listFiles(currentPath.toString());
            if (files != null) {
                for (File file : files) {
                    renderFile(file);
                }
            }

            ImGui.popStyleColor(3);

            GUIPopup.windowPopup("ExplorerWindow", () -> {
                if (ImGui.button("New File", -1, 0)) {
                    textInput = new GUITextInput("", name -> {
                        Files.createFile(currentPath + "/" + name);
                        textInput = null;
                    });
                    GUIPopup.close();
                }

                if (ImGui.button("New Script", -1, 0)) {
                    textInput = new GUITextInput("", name -> {
                        String path = currentPath + "/" + name + ".java";
                        Files.writeString(path, Script.getTemplate(name));
                        textInput = null;
                    });
                    GUIPopup.close();
                }

                if (ImGui.button("New Folder")) {
                    textInput = new GUITextInput("", name -> {
                        Files.createFolder(currentPath + "/" + name);
                        textInput = null;
                    });
                    GUIPopup.close();
                }
            });

            if (textInput != null) {
                textInput.update();
            }
        }
        ImGui.end();
    }
}
