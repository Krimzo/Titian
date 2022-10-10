package light.abs;

import editor.Editor;
import entity.Entity;
import math.Float3;
import named.NameHolder;

import java.io.Serializable;

public abstract class Light extends Entity implements Serializable {
    private final Float3 color = new Float3(1);

    public Light(NameHolder holder, String name, Editor editor) {
        super(holder, name, editor);
    }

    public void setColor(Float3 color) {
        this.color.set(color);
    }

    public Float3 getColor() {
        return new Float3(color);
    }
}
