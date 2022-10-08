package scene;

import camera.abs.Camera;
import entity.Entity;
import glparts.Mesh;
import glparts.Texture;
import glparts.abs.Disposable;
import glparts.Shaders;
import material.Material;
import named.NameHolder;
import physics.Physical;

import java.io.*;
import java.util.*;

public class Scene extends ArrayList<Entity> implements Physical, Disposable, Serializable {
    public final NameHolder entityNames = new NameHolder();
    public final NameHolder materialNames = new NameHolder();
    public final NameHolder textureNames = new NameHolder();
    public final NameHolder meshNames = new NameHolder();

    public final Set<Material> materials = new HashSet<>();
    public final Set<Texture> textures = new HashSet<>();
    public final Set<Mesh> meshes = new HashSet<>();

    public Entity selectedEntity = null;
    public Camera mainCamera = null;

    public Scene() {}

    @Override
    public void dispose() {
        for (Texture texture : textures) {
            texture.dispose();
        }
        for (Mesh mesh : meshes) {
            mesh.dispose();
        }

        this.clear();
        entityNames.clear();
        materialNames.clear();
        textureNames.clear();
        meshNames.clear();

        materials.clear();
        textures.clear();
        meshes.clear();

        selectedEntity = null;
    }

    public boolean addUnsaved(Entity entity) {
        if (entity != null) {
            return super.add(entity);
        }
        return false;
    }

    public boolean add(Entity entity) {
        boolean added = addUnsaved(entity);

        if (added) {
            if (entity.materialComponent.material != null) {
                materials.add(entity.materialComponent.material);

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

            if (entity.meshComponent.mesh != null) {
                meshes.add(entity.meshComponent.mesh);
            }
        }

        return added;
    }

    @Override
    public void updatePhysics(float deltaT) {
        for (Entity entity : this) {
            entity.updatePhysics(deltaT);
        }
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

    public static Scene fromFile(String filepath) {
        Scene scene = null;
        try {
            FileInputStream fileStream = new FileInputStream(filepath);
            ObjectInputStream objectStream = new ObjectInputStream(fileStream);
            scene = (Scene) objectStream.readObject();
            objectStream.close();
            fileStream.close();
        }
        catch (Exception e) {
            e.printStackTrace();
        }
        return scene;
    }
}
