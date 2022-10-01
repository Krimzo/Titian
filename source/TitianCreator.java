import editor.Editor;
import entity.Entity;
import glparts.*;
import material.Material;
import math.Float3;
import scene.Scene;

import java.util.Random;

public class TitianCreator {
    public static void saveTestScene(Editor editor) throws Exception {
        Scene scene = new Scene();

        Mesh monkeMesh = new Mesh(scene.meshNames, "Monke Mesh", editor.window.getContext(), "resource/meshes/monke.obj");
        Material monkeMaterial = new Material(scene.materialNames, "Monke Material");
        monkeMaterial.colorMap = new Texture(scene.textureNames, "Checkers Texture", editor.window.getContext(), "resource/textures/checkers.png");

        final int size = 3;
        for (int y = -size; y <= size; y++) {
            for (int x = -size; x <= size; x++) {
                Entity monke = new Entity(scene.entityNames, "Monke");

                monke.transformComponent.position = new Float3(x, y, 0).mul(2.5f);
                monke.physicsComponent.angular.y = (new Random().nextFloat() * 2 - 1) * 36;

                monke.meshComponent.mesh = monkeMesh;
                monke.materialComponent.material = monkeMaterial;

                scene.add(monke);
            }
        }

        scene.toFile("resource/scenes/test.scene");
        scene.dispose();
    }

    public static void loadTestScene(Editor editor) {
        editor.destroyCurrentScene();
        editor.scene = Scene.fromFile("resource/scenes/test.scene");
    }

    public static void main(String[] args) throws Exception {
        Editor editor = new Editor();

        System.setOut(editor.logger);
        System.setErr(editor.logger);

        editor.setup((ed) -> {
            try {
                saveTestScene(ed);
                loadTestScene(ed);
            }
            catch (Exception e) {
                e.printStackTrace();
            }
        });

        while (editor.process()) {
            editor.update(ignored -> {});
        }

        editor.dispose();
    }
}
