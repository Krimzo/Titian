package script;

import scene.Scene;
import script.abs.Scriptable;

public class Scripter {
    public Scripter() {}

    public void callStarts(Scene scene) {
        if (scene != null) {
            for (Scriptable scriptable : scene) {
                scriptable.scriptStart();
            }
        }
    }

    public void callUpdates(Scene scene) {
        if (scene != null) {
            for (Scriptable scriptable : scene) {
                scriptable.scriptUpdate();
            }
        }
    }
}
