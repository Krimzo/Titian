import script.abs.Scriptable;
import entity.Entity;
import window.input.*;
import java.awt.Color;
import math.*;

import camera.abs.Camera;

public class Dash extends Scriptable {
    public Dash(Entity entity) {
        super(entity);
    }

    @Override
    public void start() {

    }

    @Override
    public void update() {
        if (Input.isKeyPressed(Key.Space)) {
            Camera camera = getSceneCamera();

            Plane plane = new Plane(Float3.getPosY(), entity.components.transform.position);
            Ray ray = new Ray(camera.components.transform.position, camera.getForward());

            Float3 intersection = new Float3();
            ray.intersect(plane, intersection);

            this.entity.components.transform.position.set(intersection);
        }
    }
}
