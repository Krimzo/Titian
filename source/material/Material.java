package material;

import callbacks.EmptyCallback;
import interfaces.Disposable;
import glparts.Texture;
import math.Float3;

import java.io.Serializable;

public class Material implements Disposable, Serializable {
    public Float3 color = new Float3(1);
    public float roughness = 0.5f;

    public Texture colorMap = null;
    public Texture normalMap = null;
    public Texture roughnessMap = null;

    public Material() {}

    public Material(Texture colorMap) {
        this.colorMap = colorMap;
    }

    public void use(EmptyCallback callback) {
        int[] textures = new int[3];
        textures[0] = (colorMap != null) ? colorMap.getBuffer() : 0;
        textures[1] = (normalMap != null) ? normalMap.getBuffer() : 0;
        textures[2] = (roughnessMap != null) ? roughnessMap.getBuffer() : 0;
        Texture.use(textures, callback);
    }

    @Override
    public void dispose() {
        if (colorMap != null) {
            colorMap.dispose();
            colorMap = null;
        }
        if (normalMap != null) {
            normalMap.dispose();
            normalMap = null;
        }
        if (roughnessMap != null) {
            roughnessMap.dispose();
            roughnessMap = null;
        }
    }
}
