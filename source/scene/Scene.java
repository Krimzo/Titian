package scene;

import entity.Entity;
import interfaces.Disposable;
import glparts.Shaders;
import named.NameHolder;
import interfaces.Physical;
import interfaces.Renderable;

import java.io.*;
import java.util.ArrayList;

public class Scene extends ArrayList<Entity> implements Physical, Renderable, Disposable, Serializable {
    public final NameHolder entityNameHolder = new NameHolder();
    public final NameHolder meshNameHolder = new NameHolder();
    public final NameHolder textureNameHolder = new NameHolder();
    public final NameHolder materialNameHolder = new NameHolder();

    public transient Entity selectedEntity = null;

    public Scene() {}

    public Scene(String filepath) {
        fromFile(filepath);
    }

    @Override
    public void dispose() {
        for (Entity entity : this) {
            entity.dispose();
        }
        this.clear();

        entityNameHolder.dispose();
        meshNameHolder.dispose();
        textureNameHolder.dispose();
        materialNameHolder.dispose();

        selectedEntity = null;
    }

    public void toFile(String filepath) {
        try {
            FileOutputStream fileStream = new FileOutputStream(filepath);
            ObjectOutputStream objectStream = new ObjectOutputStream(fileStream);
            objectStream.writeObject(this);
            objectStream.close();
            fileStream.close();
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void fromFile(String filepath) {
        this.clear();

        try {
            FileInputStream fileStream = new FileInputStream(filepath);
            ObjectInputStream objectStream = new ObjectInputStream(fileStream);

            this.addAll((Scene) objectStream.readObject());

            objectStream.close();
            fileStream.close();
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Override
    public void updatePhysics(float deltaT) {
        for (Entity entity : this) {
            entity.updatePhysics(deltaT);
        }
    }

    @Override
    public void render(Shaders shaders) {
        for (Entity entity : this) {
            entity.render(shaders);
        }
    }
}
