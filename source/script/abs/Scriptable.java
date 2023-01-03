package script.abs;

import camera.abs.Camera;
import editor.Editor;
import entity.Entity;
import light.AmbientLight;
import light.DirectionalLight;
import logging.LogInfo;
import logging.LogType;
import math.Float3;
import math.Int2;

public abstract class Scriptable {
    protected Entity self;

    protected Scriptable(Entity self) {
        this.self = self;
    }

    public abstract void start();

    public abstract void update();

    protected void log(Object object) {
        String sender = self.getName() + " [" + this.getClass().getName() + "]";
        editor.getLogger().log(new LogInfo(LogType.SCRIPT, sender, object));
    }

    protected Int2 getWindowPosition() {
        return editor.getWindow().getPosition();
    }

    protected Int2 getWindowSize() {
        return editor.getWindow().getSize();
    }

    protected void setVSync(Boolean enabled) {
        editor.getWindow().setVSync(enabled);
    }

    protected Int2 getMousePosition() {
        return editor.getWindow().getMousePosition();
    }

    protected float getDeltaT() {
        return editor.getTimer().getDeltaT();
    }

    protected float getElapsedT() {
        return editor.getTimer().getElapsedT();
    }

    protected Camera getSceneCamera() {
        return editor.getScene().getSelected().getCamera();
    }

    protected AmbientLight getSceneAmbientLight() {
        return editor.getScene().getSelected().getAmbientLight();
    }

    protected DirectionalLight getSceneDirectionalLight() {
        return editor.getScene().getSelected().getDirectionalLight();
    }

    // Editor linking
    private static Editor editor = null;

    public static void setEditor(Editor editor) {
        Scriptable.editor = editor;
    }
}
