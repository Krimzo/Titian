import editor.Editor;
import entity.Entity;
import glparts.*;
import material.Material;
import scene.Scene;

import java.util.Random;

public class TitianCreator {
    public static void saveTestScene(Editor editor) throws Exception {
        Scene scene = new Scene();

        Mesh monkeMesh = new Mesh(scene.meshNames, "Monke Mesh", editor.window.getContext(), "resource/meshes/monke.obj");
        Material monkeMaterial = new Material(scene.materialNames, "Monke Material");
        monkeMaterial.colorMap = new Texture(scene.textureNames, "Checkers Texture", editor.window.getContext(), "resource/textures/checkers.png");

        for (int x = -3; x <= 3; x++) {
            Entity monke = new Entity(scene.entityNames, "Monke");

            monke.transformComponent.position.x = x * 2.5f;
            monke.physicsComponent.angular.y = (new Random().nextFloat() * 2 - 1) * 36;

            monke.meshComponent.mesh = monkeMesh;
            monke.materialComponent.material = monkeMaterial;

            scene.add(monke);
        }

        scene.toFile("resource/scenes/test.scene");
        scene.dispose();
    }

    public static void loadTestScene(Editor editor) throws Exception {
        editor.changeScene(Scene.fromFile("resource/scenes/test.scene"));
    }

    public static void setup(Editor editor) {
        try {
            saveTestScene(editor);
            loadTestScene(editor);
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) throws Exception {
        Editor editor = new Editor();
        editor.setup(TitianCreator::setup);

        while (editor.process()) {
            editor.update(ignored -> {});
        }

        editor.dispose();
    }
}
