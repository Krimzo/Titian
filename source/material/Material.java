package material;

import callback.EmptyCallback;
import glparts.Shaders;
import glparts.Texture;
import math.Float3;
import named.NameHolder;
import named.Named;
import utility.Instance;

import java.io.Serializable;

public class Material extends Named implements Serializable {
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
        textures[0] = Instance.isValid(colorMap) ? colorMap.getBuffer() : 0;
        textures[1] = Instance.isValid(normalMap) ? normalMap.getBuffer() : 0;
        textures[2] = Instance.isValid(roughnessMap) ? roughnessMap.getBuffer() : 0;
        Texture.use(textures, callback);
    }

    public void updateUniforms(Shaders shaders) {
        final boolean useColorMap = Instance.isValid(colorMap);
        shaders.setUniform("color", color);
        shaders.setUniform("useColorMap", useColorMap ? 1 : 0);
        shaders.setUniform("colorMap", 0);
    }
}
