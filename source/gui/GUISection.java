package gui;

import editor.Editor;

public abstract class GUISection {
    protected Editor editor;

    protected GUISection(Editor editor) {
        this.editor = editor;
    }
}
