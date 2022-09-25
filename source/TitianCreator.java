import editor.Editor;
import entity.Entity;
import glparts.*;
import gui.sections.GUIExplorer;
import gui.sections.GUIProperties;
import gui.sections.GUIScene;
import gui.sections.GUIViewport;
import renderer.Renderable;
import scene.Scene;

import java.util.ArrayList;
import java.util.Random;

public class TitianCreator {
    public static void setup(Editor editor) throws Exception {
        editor.userData.put("WireframeState", false);
        editor.window.keyboard.v.onPress = () -> {
            boolean newState = !(boolean) editor.userData.get("WireframeState");
            editor.userData.put("WireframeState", newState);
            editor.context.setWireframe(newState);
        };

        Mesh monkeMesh = new Mesh(editor.context, "resource/meshes/monke.obj");
        Texture checkersTexture = new Texture(editor.context, "resource/textures/checkers.png");
        //editor.updateScene(new Scene("resource/scenes/Test.scene"));

        if (true) {
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
            System.exit(0);
        }

        guiSetup(editor);
    }

    public static void guiSetup(Editor editor) {
        editor.guiRenderer.add(new GUIScene());
        editor.guiRenderer.add(new GUIViewport());
        editor.guiRenderer.add(new GUIExplorer());
        editor.guiRenderer.add(new GUIProperties());
    }

    public static void update(Editor editor) {

    }

    public static void main(String[] args) throws Exception {
        Editor editor = new Editor();
        editor.setup(TitianCreator::setup);

        while (editor.process()) {
            editor.update(TitianCreator::update);
        }

        editor.destroy();
    }
}
