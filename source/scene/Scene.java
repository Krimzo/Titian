package scene;

import editor.Editor;
import entity.Entity;
import glparts.Mesh;
import glparts.Texture;
import material.Material;
import physics.Physical;
import utility.Instance;
import utility.abs.Allocated;
import utility.nncollection.NNArrayList;
import utility.nncollection.NNHashSet;

import java.io.*;
import java.util.Set;

public class Scene extends NNArrayList<Entity> implements Physical, Allocated, Serializable {
    public final Set<Material> materials = new NNHashSet<>();
    public final Set<Texture> textures = new NNHashSet<>();
    public final Set<Mesh> meshes = new NNHashSet<>();

    public final SceneNameHolders names = new SceneNameHolders();
    public final SceneSelectedData selected = new SceneSelectedData();

    public Scene() {}

    @Override
    public void free() {
        for (Texture texture : textures) {
            texture.free();
        }
        for (Mesh mesh : meshes) {
            mesh.free();
        }
        materials.clear();
        textures.clear();
        meshes.clear();
        this.clear();

        names.free();
        selected.free();
    }

    @Override
    public boolean add(Entity entity) {
        if (super.add(entity)) {
            meshes.add(entity.components.mesh.mesh);
            materials.add(entity.components.material.material);

            if (Instance.isValid(entity.components.material.material)) {
                textures.add(entity.components.material.material.colorMap);
                textures.add(entity.components.material.material.normalMap);
                textures.add(entity.components.material.material.roughnessMap);
            }

            return true;
        }
        return false;
    }

    @Override
    public void updatePhysics(float deltaT) {
        for (Entity entity : this) {
            entity.updatePhysics(deltaT);
        }
    }

    public void toFile(String filepath) {
        try {
            boolean ignored = new File(filepath).getParentFile().mkdirs();
            try (ObjectOutputStream stream = new ObjectOutputStream(new FileOutputStream(filepath))) {
                stream.writeObject(this);
            }
        }
        catch (Exception ignored) {
            System.out.println("Scene saving error, \"" + filepath + '"');
        }
    }

    public static Scene fromFile(String filepath, Editor editor) {
        Scene scene = null;
        try (ObjectInputStream stream = new ObjectInputStream(new FileInputStream(filepath))) {
            scene = (Scene) stream.readObject();
            for (Entity entity : scene) {
                entity.editor = editor;
            }
        }
        catch (Exception ignored) {
            System.out.println("Scene loading error, \"" + filepath + '"');
        }
        return scene;
    }
}
