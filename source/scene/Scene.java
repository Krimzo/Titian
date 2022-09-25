package scene;

import entity.Entity;
import glparts.Shaders;
import physics.Physical;
import renderer.Renderable;

import java.io.*;
import java.util.ArrayList;

public class Scene extends ArrayList<Entity> implements Physical, Renderable, Serializable {
    public Scene() {}

    public Scene(String filepath) {
        fromFile(filepath);
    }

    public void toFile(String filepath) {
        try {
            FileOutputStream fileStream = new FileOutputStream(filepath);
            ObjectOutputStream objectStream = new ObjectOutputStream(fileStream);
            objectStream.writeObject(this);
            objectStream.close();
            fileStream.close();
        }
        catch (Exception ignored) {
            ignored.printStackTrace();
        }
    }

    public void fromFile(String filepath) {
        this.clear();
        try {
            FileInputStream fileStream = new FileInputStream(filepath);
            ObjectInputStream objectStream = new ObjectInputStream(fileStream);
            Scene scene = (Scene) objectStream.readObject();
            objectStream.close();
            fileStream.close();

            for (Entity entity : scene) {
                entity.loadSerialization();
                this.add(entity);
            }
        }
        catch (Exception ignored) {}
    }

    @Override
    public void onPhysicsUpdate(float deltaT) {
        for (Entity entity : this) {
            entity.onPhysicsUpdate(deltaT);
        }
    }

    @Override
    public void onRender(Shaders shaders) {
        for (Entity entity : this) {
            entity.onRender(shaders);
        }
    }
}
