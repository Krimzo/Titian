import script.abs.Scriptable;
import entity.Entity;
import window.input.*;
import java.awt.Color;
import math.*;

import camera.abs.Camera;

public class Move extends Scriptable {
    public Move(Entity entity) {
        super(entity);
    }

    @Override
    public void start() {

    }

    @Override
    public void update() {
        if (self instanceof Camera) {
            ((Camera) self).useDefaultMovement(Mouse.Right, Key.W, Key.S, Key.D, Key.A, Key.E, Key.Q,
                2, getWindowSize(), getDeltaT()
            );
        }
    }
}

