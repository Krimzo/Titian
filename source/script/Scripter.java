package script;

import entity.Entity;
import scene.Scene;
import utility.Instance;

public class Scripter {
    public Scripter() {}

    public void callStarts(Scene scene) {
        if (Instance.isValid(scene)) {
            for (Entity entity : scene) {
                entity.callScriptStarts();
            }
        }
    }

    public void callUpdates(Scene scene) {
        if (Instance.isValid(scene)) {
            for (Entity entity : scene) {
                entity.callScriptUpdates();
            }
        }
    }
}
