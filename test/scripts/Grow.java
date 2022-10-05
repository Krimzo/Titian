import script.abs.Scriptable;
import entity.Entity;
import math.*;

public class Grow extends Scriptable {
    public Grow(Entity entity) {
        super(entity);
    }

    @Override
    public void start() {

    }

    @Override
    public void update() {
        entity.transformComponent.scale = new Float3(editor.timer.getElapsedT() * 0.1f);
    }
}
