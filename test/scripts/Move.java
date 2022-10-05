import script.abs.Scriptable;
import entity.Entity;
import math.*;

public class Move extends Scriptable {
    private float velocity = 2.0f;

    public Move(Entity entity) {
        super(entity);
    }

    @Override
    public void start() {
        editor.window.keyboard.i.onHold.add(() -> {
            entity.transformComponent.position.z -= velocity * editor.timer.getDeltaT();
        });
        editor.window.keyboard.k.onHold.add(() -> {
            entity.transformComponent.position.z += velocity * editor.timer.getDeltaT();
        });
        editor.window.keyboard.j.onHold.add(() -> {
            entity.transformComponent.position.x -= velocity * editor.timer.getDeltaT();
        });
        editor.window.keyboard.l.onHold.add(() -> {
            entity.transformComponent.position.x += velocity * editor.timer.getDeltaT();
        });
        editor.window.keyboard.u.onHold.add(() -> {
            entity.transformComponent.position.y -= velocity * editor.timer.getDeltaT();
        });
        editor.window.keyboard.o.onHold.add(() -> {
            entity.transformComponent.position.y += velocity * editor.timer.getDeltaT();
        });
    }

    @Override
    public void update() {

    }
}
