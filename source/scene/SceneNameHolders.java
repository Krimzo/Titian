package scene;

import named.NameHolder;

import java.io.Serializable;

public class SceneNameHolders implements Serializable {
    public final NameHolder entity = new NameHolder();
    public final NameHolder mesh = new NameHolder();
    public final NameHolder material = new NameHolder();
    public final NameHolder texture = new NameHolder();

    public SceneNameHolders() {}
}
