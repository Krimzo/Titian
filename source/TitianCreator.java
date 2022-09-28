import editor.Editor;
import entity.Entity;
import glparts.*;
import scene.Scene;

import java.util.Random;

public class TitianCreator {
    public static String randomMesh() {
        String path = "resource/meshes/default/";
        String[] meshes = {
            "capsule.obj",
            "cube.obj",
            "monke.obj",
            "pyramid.obj",
            "sphere.obj",
        };
        return path + meshes[new Random().nextInt(meshes.length)];
    }

    public static String randomTexture() {
        String path = "resource/textures/";
        String[] textures = {
            "checkers.png",
            "dogo.png",
        };
        return path + textures[new Random().nextInt(textures.length)];
    }

    public static void setup(Editor editor) {
        try {
            /* Scene saving */ {
                Scene scene = new Scene();
                Mesh mesh = new Mesh(editor.window.getContext(), randomMesh());
                Texture texture = new Texture(editor.window.getContext(), randomTexture());

                for (int x = -3; x <= 3; x++) {
                    Entity monke = new Entity(scene.entityNameHolder);

                    monke.setName("Object");
                    monke.meshComponent.mesh = mesh;
                    monke.materialComponent.material.colorMap = texture;
                    monke.transformComponent.position.x = x * 2.5f;
                    monke.physicsComponent.angular.y = (new Random().nextFloat() * 2 - 1) * 36;

                    scene.add(monke);
                }

                scene.toFile("resource/scenes/test.scene");
                scene.dispose();
            }

            /* Scene loading */ {
                editor.changeScene(new Scene("resource/scenes/test.scene"));
            }
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void update(Editor editor) {}

    public static void main(String[] args) throws Exception {
        Editor editor = new Editor();
        editor.setup(TitianCreator::setup);

        while (editor.process()) {
            editor.update(TitianCreator::update);
        }

        editor.dispose();
    }
}
