package scene;

import glparts.abs.Disposable;
import named.NameHolder;

import java.io.Serializable;

public class SceneNameHolders implements Disposable, Serializable {
    public final NameHolder entity = new NameHolder();
    public final NameHolder mesh = new NameHolder();
    public final NameHolder material = new NameHolder();
    public final NameHolder texture = new NameHolder();

    public SceneNameHolders() {}

    @Override
    public void dispose() {
        entity.clear();
        mesh.clear();
        material.clear();
        texture.clear();
    }
}
