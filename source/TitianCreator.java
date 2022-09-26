import editor.Editor;
import entity.Entity;
import glparts.*;
import scene.Scene;

import java.util.Random;

public class TitianCreator {
    public static void setup(Editor editor) throws Exception {
        // Scene saving
        Mesh monkeMesh = new Mesh(editor.context, "resource/meshes/monke.obj");
        Texture checkersTexture = new Texture(editor.context, "resource/textures/checkers.png");

        final boolean saveTestScene = true;
        if (saveTestScene) {
            Scene scene = new Scene();
            for (int x = -3; x <= 3; x++) {
                Entity monke = new Entity();
                monke.meshComponent.mesh = monkeMesh;
                monke.materialComponent.material.colorMap = checkersTexture;
                monke.transformComponent.position.x = x * 2.5f;
                monke.physicsComponent.angular.y = (new Random().nextFloat() * 2 - 1) * 36;
                scene.add(monke);
            }
            scene.toFile("resource/scenes/Test.scene");
        }

        // Scene loading
        editor.setScene(new Scene("resource/scenes/Test.scene"));
    }

    public static void update(Editor editor) {}

    public static void main(String[] args) throws Exception {
        Editor.DEBUG = true;

        Editor editor = new Editor();
        editor.setup(TitianCreator::setup);

        while (editor.process()) {
            editor.update(TitianCreator::update);
        }

        editor.destroy();
    }
}
