package scene;

import camera.abs.Camera;
import editor.Editor;
import entity.Entity;
import glparts.Mesh;
import glparts.Texture;
import glparts.abs.Disposable;
import material.Material;
import named.NameHolder;
import physics.Physical;
import utility.nncollection.*;

import java.io.*;
import java.util.*;

public class Scene extends NNArrayList<Entity> implements Physical, Disposable, Serializable {
    public final NameHolder entityNames = new NameHolder();
    public final NameHolder materialNames = new NameHolder();
    public final NameHolder textureNames = new NameHolder();
    public final NameHolder meshNames = new NameHolder();

    public final Set<Material> materials = new NNHashSet<>();
    public final Set<Texture> textures = new NNHashSet<>();
    public final Set<Mesh> meshes = new NNHashSet<>();

    public Camera camera = null;
    public Entity selected = null;

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

        camera = null;
        selected = null;
    }

    public boolean addUnsaved(Entity entity) {
        return super.add(entity);
    }

    @Override
    public boolean add(Entity entity) {
        boolean added = addUnsaved(entity);

        if (added) {
            meshes.add(entity.components.mesh.mesh);

            if (entity.components.material.material != null) {
                materials.add(entity.components.material.material);
                textures.add(entity.components.material.material.colorMap);
                textures.add(entity.components.material.material.normalMap);
                textures.add(entity.components.material.material.roughnessMap);
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
        catch (Exception ignored) {
            System.out.println("Scene saving error, \"" + filepath + '"');
        }
    }

    public static Scene fromFile(String filepath, Editor editor) {
        Scene scene = null;
        try {
            FileInputStream fileStream = new FileInputStream(filepath);
            ObjectInputStream objectStream = new ObjectInputStream(fileStream);

            scene = (Scene) objectStream.readObject();
            for (Entity entity : scene) {
                entity.editor = editor;
            }

            objectStream.close();
            fileStream.close();
        }
        catch (Exception ignored) {
            System.out.println("Scene loading error, \"" + filepath + '"');
        }
        return scene;
    }
}
