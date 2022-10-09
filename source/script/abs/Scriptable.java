package script.abs;

import camera.abs.Camera;
import editor.Editor;
import entity.Entity;
import logging.*;
import math.*;

public abstract class Scriptable {
    private static Editor editor = null;
    protected Entity entity = null;

    public Scriptable(Entity entity) {
        this.entity = entity;
    }

    public abstract void start();
    public abstract void update();

    public static void setEditor(Editor editor) {
        Scriptable.editor = editor;
    }

    public void log(Object object) {
        String sender = entity.getName() + " [" + this.getClass().getName() + "]";
        editor.logger.log(new LogInfo(LogType.SCRIPT, sender, object));
    }

    public Int2 getWindowPosition() {
        return editor.window.getPosition();
    }

    public Int2 getWindowSize() {
        return editor.window.getSize();
    }

    public void setVSync(boolean enabled) {
        editor.window.setVSync(enabled);
    }

    public Int2 getMousePosition() {
        return editor.window.getMousePosition();
    }

    public float getDeltaT() {
        return editor.timer.getDeltaT();
    }

    public float getElapsedT() {
        return editor.timer.getElapsedT();
    }

    public Camera getSceneCamera() {
        return editor.scene.camera;
    }
}
