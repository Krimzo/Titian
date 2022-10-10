package light;

import editor.Editor;
import light.abs.Light;
import math.Float3;
import named.NameHolder;

import java.io.Serializable;

public class DirectionalLight extends Light implements Serializable {
    private final Float3 direction = Float3.getNegZ();

    public DirectionalLight(NameHolder holder, String name, Editor editor) {
        super(holder, name, editor);
    }

    public void setDirection(Float3 direction) {
        this.direction.set(direction.normalize());
    }

    public Float3 getDirection() {
        return new Float3(direction);
    }
}
