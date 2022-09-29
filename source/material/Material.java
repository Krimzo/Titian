package material;

import callback.EmptyCallback;
import utility.Disposable;
import glparts.Texture;
import math.Float3;
import named.NameHolder;
import named.Named;

import java.io.Serializable;

public class Material extends Named implements Disposable, Serializable {
    public final Float3 color = new Float3(1);
    public float roughness = 0.5f;

    public Texture colorMap = null;
    public Texture normalMap = null;
    public Texture roughnessMap = null;

    public Material(NameHolder holder, String name) {
        super(holder, name);
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
