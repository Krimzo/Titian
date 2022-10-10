package light;

import editor.Editor;
import light.abs.Light;
import math.Float3;
import named.NameHolder;

import java.io.Serializable;

public class PositionalLight extends Light implements Serializable {
    public float intensity = 1;

    public PositionalLight(NameHolder holder, String name, Editor editor) {
        super(holder, name, editor);
    }

    @Override
    public Float3 getColor() {
        return super.getColor().multiply(intensity);
    }
}
