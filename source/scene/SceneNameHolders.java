package scene;

import named.NameHolder;
import utility.abs.Allocated;

import java.io.Serializable;

public class SceneNameHolders implements Allocated, Serializable {
    public final NameHolder entity = new NameHolder();
    public final NameHolder mesh = new NameHolder();
    public final NameHolder material = new NameHolder();
    public final NameHolder texture = new NameHolder();

    public SceneNameHolders() {}

    @Override
    public void free() {
        entity.clear();
        mesh.clear();
        material.clear();
        texture.clear();
    }
}
