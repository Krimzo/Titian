package script;

import entity.Entity;
import scene.Scene;

public class ScriptEngine {
    public ScriptEngine() {}

    public void callStarts(Scene scene) {
        for (Entity entity : scene) {
            entity.callScriptStarts();
        }
    }

    public void callUpdates(Scene scene) {
        for (Entity entity : scene) {
            entity.callScriptUpdates();
        }
    }
}
