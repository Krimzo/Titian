import script.abs.Scriptable;
import entity.Entity;
import window.input.*;
import java.awt.Color;
import math.*;

import camera.PerspectiveCamera;

public class Orbit extends Scriptable {
    public float radius = 5.0f;

    public Orbit(Entity entity) {
        super(entity);
    }

    @Override
    public void start() {
        
    }

    @Override
    public void update() {
        if (self instanceof PerspectiveCamera) {
            var castedSelf = (PerspectiveCamera) self;
            
            // Position
            var position = castedSelf.getComponents().getTransform().getPosition();
            position.setX((float) Math.sin(getElapsedT()) * radius);
            position.setZ((float) Math.cos(getElapsedT()) * radius);
            
            // Direction
            var direction = position.unaryMinus();
            castedSelf.setForward(direction);
        }
    }
}


