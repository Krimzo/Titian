import script.abs.Scriptable;
import entity.Entity;
import java.awt.Color;
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
        entity.transformComponent.scale.set(new Float3(editor.timer.getElapsedT() * 0.1f));
    }
}
