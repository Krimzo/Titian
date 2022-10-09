import script.abs.Scriptable;
import entity.Entity;
import window.input.*;
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
        entity.components.transform.scale.set(new Float3(getElapsedT() * 0.1f));
    }
}
