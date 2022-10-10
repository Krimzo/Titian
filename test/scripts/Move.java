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
        Camera camera = (Camera) this.entity;
        camera.useDefaultMovement(Mouse.Right, Key.W, Key.S, Key.D, Key.A, Key.E, Key.Q,
            getWindowSize(), getDeltaT()
        );
    }
}
