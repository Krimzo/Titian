package script;

import entity.Entity;
import scene.Scene;

public class Scripter {
    public Scripter() {}

    public void callStarts(Scene scene) {
        if (scene != null) {
            for (Entity entity : scene) {
                entity.callScriptStarts();
            }
        }
    }

    public void callUpdates(Scene scene) {
        if (scene != null) {
            for (Entity entity : scene) {
                entity.callScriptUpdates();
            }
        }
    }
}
