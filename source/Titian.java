import editor.Editor;
import entity.Entity;
import entity.material.Material;
import glparts.Mesh;
import glparts.Texture;
import light.AmbientLight;
import light.DirectionalLight;
import math.Float2;
import math.Float3;
import scene.Scene;
import script.abs.Scriptable;

import java.util.Random;

public class Titian {
    public static void saveTestScene(Editor editor) throws Exception {
        Scene scene = new Scene();

        scene.selected.ambientLight = new AmbientLight(scene.names.entity, "Ambient Light", editor);
        scene.selected.directionalLight = new DirectionalLight(scene.names.entity, "Directional Light", editor);
        scene.selected.directionalLight.setDirection(new Float3(1, -1, -1));
        scene.add(scene.selected.ambientLight);
        scene.add(scene.selected.directionalLight);

        Mesh monkeMesh = new Mesh(scene.names.mesh, "Monke Mesh", editor.window.getContext(), "resource/meshes/monke.obj");
        Material monkeMaterial = new Material(scene.names.material, "Monke Material");
        monkeMaterial.colorMap = new Texture(scene.names.texture, "Checkers Texture", editor.window.getContext(), "test/textures/checkers.png");
        monkeMaterial.colorBlend = 1;

        final int size = 1;
        for (int y = -size; y <= size; y++) {
            for (int x = -size; x <= size; x++) {
                Entity monke = new Entity(scene.names.entity, "Monke", editor);

                monke.components.transform.position.set(new Float2(x, y).multiply(2.5f), 0);
                monke.components.physics.angular.y = (new Random().nextFloat() * 2 - 1) * 36;

                monke.components.mesh.mesh = monkeMesh;
                monke.components.material.material = monkeMaterial;

                scene.add(monke);
            }
        }

        scene.toFile("test/scenes/test.titian");
    }

    public static void loadTestScene(Editor editor) {
        editor.setScene(Scene.fromFile("test/scenes/test.titian", editor));
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

        editor.destroy();
    }
}
