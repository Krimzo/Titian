import editor.Editor;
import entity.Entity;
import glparts.Mesh;
import glparts.Texture;
import material.Material;
import math.Float2;
import scene.Scene;
import script.abs.Scriptable;

import java.util.Random;

public class Titian {
    public static void saveTestScene(Editor editor) throws Exception {
        Scene scene = new Scene();

        Mesh monkeMesh = new Mesh(scene.meshNames, "Monke Mesh", editor.window.getContext(), "resource/meshes/monke.obj");
        Material monkeMaterial = new Material(scene.materialNames, "Monke Material");
        monkeMaterial.colorMap = new Texture(scene.textureNames, "Checkers Texture", editor.window.getContext(), "test/textures/checkers.png");

        final int size = 1;
        for (int y = -size; y <= size; y++) {
            for (int x = -size; x <= size; x++) {
                Entity monke = new Entity(scene.entityNames, "Monke", editor);

                monke.components.transform.position.set(new Float2(x, y).multiply(2.5f), 0);
                monke.components.physics.angular.y = (new Random().nextFloat() * 2 - 1) * 36;

                monke.components.mesh.mesh = monkeMesh;
                monke.components.material.material = monkeMaterial;

                scene.add(monke);
            }
        }

        scene.toFile("test/scenes/test.scene");
        scene.dispose();
    }

    public static void loadTestScene(Editor editor) {
        editor.disposeCurrentScene();
        editor.scene = Scene.fromFile("test/scenes/test.scene", editor);
    }

    public static void main(String[] args) throws Exception {
        Editor editor = new Editor();

        System.setOut(editor.logger);
        System.setErr(editor.logger);

        Scriptable.setEditor(editor);

        editor.setup((ed) -> {
            try {
                saveTestScene(ed);
                loadTestScene(ed);
            }
            catch (Exception ignored) {
                System.out.println("Editor setup error");
            }
        });

        while (editor.window.process()) {
            editor.update();
        }

        editor.dispose();
    }
}
