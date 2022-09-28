package gui;

import editor.Editor;
import interfaces.GUIRenderable;

public abstract class GUISection implements GUIRenderable {
    protected Editor editor;

    protected GUISection(Editor editor) {
        this.editor = editor;
    }
}
