package gui.abs;

import editor.Editor;
import gui.abs.GUIRenderable;

public abstract class GUISection implements GUIRenderable {
    protected Editor editor;

    protected GUISection(Editor editor) {
        this.editor = editor;
    }
}
