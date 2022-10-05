package script.abs;

import editor.Editor;
import entity.Entity;
import logging.LogInfo;
import logging.LogType;

public abstract class Scriptable {
    public static Editor editor = null;
    public Entity entity = null;

    public Scriptable(Entity entity) {
        this.entity = entity;
    }

    public void log(Object object) {
        String sender = entity.getName() + " [" + this.getClass().getName() + "]";
        editor.logger.log(new LogInfo(LogType.SCRIPT, sender, object));
    }

    public abstract void start();
    public abstract void update();
}
