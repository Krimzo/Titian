package material;

import glparts.Texture;
import math.Float3;

import java.io.Serializable;

public class Material implements Serializable {
    public Float3 color = new Float3(1);
    public float roughness = 0;

    public Texture colorMap = null;
    public Texture normalMap = null;
    public Texture roughnessMap = null;

    public Material() {}

    public Material(Texture colorMap) {
        this.colorMap = colorMap;
    }

    public void bind() {
        if (colorMap != null) colorMap.bind(0);
        if (normalMap != null) normalMap.bind(1);
        if (roughnessMap != null) roughnessMap.bind(2);
    }
}
