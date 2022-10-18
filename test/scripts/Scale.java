import script.abs.Scriptable;
import entity.Entity;
import window.input.*;
import java.awt.Color;
import math.*;

public class Scale extends Scriptable {
    private Float3 originalScale = new Float3(1);
    public float multiplier = 0.5f;

    public Scale(Entity entity) {
        super(entity);
    }

    @Override
    public void start() {
        originalScale = new Float3(self.components.transform.scale);
    }

    @Override
    public void update() {
        Float3 scale = self.components.transform.scale;

        if (Input.isKeyDown(Key.R)) {
            scale.x = originalScale.x * multiplier;
        }
        else {
            scale.x = originalScale.x;
        }

        if (Input.isKeyDown(Key.T)) {
            scale.y = originalScale.y * multiplier;
        }
        else {
            scale.y = originalScale.y;
        }

        if (Input.isKeyDown(Key.Y)) {
            scale.z = originalScale.z * multiplier;
        }
        else {
            scale.z = originalScale.z;
        }
    }
}
