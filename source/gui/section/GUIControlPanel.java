package gui.section;

import editor.Editor;
import gui.abs.GUISection;
import imgui.ImGui;
import imgui.extension.imguizmo.flag.Mode;
import imgui.extension.imguizmo.flag.Operation;
import imgui.flag.ImGuiStyleVar;
import imgui.flag.ImGuiTableFlags;
import imgui.flag.ImGuiWindowFlags;
import scene.Scene;

public final class GUIControlPanel extends GUISection {
    private static final String SAVED_PLAY_PATH = "./temp/play.titian";

    public GUIControlPanel(Editor editor) {
        super(editor);
    }

    private void onGameStart() {
        editor.getScene().toFile(SAVED_PLAY_PATH);
        editor.scriptEngine.callStarts(editor.getScene());
        editor.timer.reset();
    }

    private void onGameEnd() {
        editor.setScene(Scene.fromFile(SAVED_PLAY_PATH, editor));
    }

    @Override
    public void renderGUI() {
        ImGui.pushStyleVar(ImGuiStyleVar.WindowPadding, 1, 1);

        if (ImGui.begin("Control Panel", ImGuiWindowFlags.NoScrollbar)) {
            if (ImGui.beginTable("##ControlTable", 3, ImGuiTableFlags.SizingStretchSame | ImGuiTableFlags.Borders, -1, -1)) {
                ImGui.tableNextRow();

                ImGui.tableNextColumn();
                if (ImGui.checkbox("Playing", editor.data.gameRunning)) {
                    if (!editor.data.gameRunning) {
                        onGameStart();
                    } else {
                        onGameEnd();
                    }
                    editor.data.gameRunning = !editor.data.gameRunning;
                }

                ImGui.tableNextColumn();
                if (ImGui.checkbox("Wireframe", editor.data.wireframeState)) {
                    editor.data.wireframeState = !editor.data.wireframeState;
                }

                ImGui.sameLine();
                if (ImGui.checkbox("Grid", editor.data.renderGrid)) {
                    editor.data.renderGrid = !editor.data.renderGrid;
                }

                ImGui.tableNextColumn();
                if (ImGui.checkbox("Local", editor.data.gizmoMode == Mode.LOCAL)) {
                    editor.data.gizmoMode = (editor.data.gizmoMode != Mode.LOCAL) ? Mode.LOCAL : Mode.WORLD;
                }

                ImGui.sameLine();
                if (ImGui.checkbox("Scale", editor.data.gizmoOperation == Operation.SCALE)) {
                    editor.data.gizmoOperation = (editor.data.gizmoOperation != Operation.SCALE) ? Operation.SCALE : 0;
                }

                ImGui.sameLine();
                if (ImGui.checkbox("Rotation", editor.data.gizmoOperation == Operation.ROTATE)) {
                    editor.data.gizmoOperation = (editor.data.gizmoOperation != Operation.ROTATE) ? Operation.ROTATE : 0;
                }

                ImGui.sameLine();
                if (ImGui.checkbox("Translation", editor.data.gizmoOperation == Operation.TRANSLATE)) {
                    editor.data.gizmoOperation = (editor.data.gizmoOperation != Operation.TRANSLATE) ? Operation.TRANSLATE : 0;
                }

                ImGui.endTable();
            }
        }
        ImGui.end();

        ImGui.popStyleVar();
    }
}
