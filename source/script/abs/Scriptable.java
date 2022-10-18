package script.abs;

import camera.abs.Camera;
import editor.Editor;
import entity.Entity;
import light.AmbientLight;
import light.DirectionalLight;
import logging.LogInfo;
import logging.LogType;
import math.Int2;

public abstract class Scriptable {
    private static Editor editor = null;
    protected final Entity self;

    public Scriptable(Entity entity) {
        this.self = entity;
    }

    public abstract void start();
    public abstract void update();

    public static void setEditor(Editor editor) {
        Scriptable.editor = editor;
    }

    public void log(Object object) {
        String sender = self.getName() + " [" + this.getClass().getName() + "]";
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
        return editor.getScene().selected.camera;
    }

    public AmbientLight getSceneAmbientLight() {
        return editor.getScene().selected.ambientLight;
    }

    public DirectionalLight getSceneDirectionalLight() {
        return editor.getScene().selected.directionalLight;
    }
}
