package scene;

import entity.Entity;
import glparts.Mesh;
import glparts.Shaders;
import glparts.Texture;
import physics.Physical;
import renderer.Renderable;

import java.io.*;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.Set;

public class Scene extends ArrayList<Entity> implements Physical, Renderable, Serializable {
    public transient final Set<Mesh> meshes = new HashSet<>();
    public transient final Set<Texture> textures = new HashSet<>();
    public transient Entity selectedEntity = null;

    public Scene() {}

    public Scene(String filepath) {
        fromFile(filepath);
    }

    public void destroy() {
        selectedEntity = null;

        for (Entity entity : this) {
            entity.meshComponent.mesh = null;
            if (entity.materialComponent.material != null) {
                entity.materialComponent.material.colorMap = null;
                entity.materialComponent.material.normalMap = null;
                entity.materialComponent.material.roughnessMap = null;
            }
        }
        for (Texture texture : textures) {
            texture.destroy();
        }
        for (Mesh mesh : meshes) {
            mesh.destroy();
        }

        this.clear();
        textures.clear();
        meshes.clear();
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
        meshes.clear();
        textures.clear();

        try {
            FileInputStream fileStream = new FileInputStream(filepath);
            ObjectInputStream objectStream = new ObjectInputStream(fileStream);
            Scene scene = (Scene) objectStream.readObject();
            objectStream.close();
            fileStream.close();

            for (Entity entity : scene) {
                if (entity.meshComponent.mesh != null) {
                    meshes.add(entity.meshComponent.mesh);
                }

                if (entity.materialComponent.material != null) {
                    if (entity.materialComponent.material.colorMap != null) {
                        textures.add(entity.materialComponent.material.colorMap);
                    }
                    if (entity.materialComponent.material.normalMap != null) {
                        textures.add(entity.materialComponent.material.normalMap);
                    }
                    if (entity.materialComponent.material.roughnessMap != null) {
                        textures.add(entity.materialComponent.material.roughnessMap);
                    }
                }

                this.add(entity);
            }
        }
        catch (Exception e) {
            e.printStackTrace();
        }
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
