package editor;

import callback.EditorCallback;
import camera.PerspectiveCamera;
import glparts.abs.GLObject;
import gui.GUIRenderer;
import gui.section.*;
import gui.section.script.GUIScriptEditor;
import logging.Logger;
import math.Int2;
import physics.PhysicsEngine;
import renderer.EditorRenderer;
import renderer.GameRenderer;
import scene.Scene;
import script.ScriptEngine;
import utility.Instance;
import utility.Timer;
import window.Window;

public class Editor {
    public final EditorData data;

    public final Window window = new Window(new Int2(1600, 900), "Titian", true);
    public final Timer timer = new Timer();

    public final PerspectiveCamera camera = new PerspectiveCamera(null, "Editor Camera", this);
    public final EditorRenderer editorRenderer = new EditorRenderer(window.getContext(), window.getSize());
    public final GameRenderer gameRenderer = new GameRenderer(window.getContext(), window.getSize());
    public final GUIRenderer guiRenderer = new GUIRenderer(window);

    public final PhysicsEngine physicsEngine = new PhysicsEngine();

    public final Logger logger = new Logger();
    public final ScriptEngine scriptEngine = new ScriptEngine();

    private Scene scene = new Scene();

    public Editor() throws Exception {
        window.setIcon("resource/textures/titian.png");
        window.getContext().setDepthTest(true);
        window.setVSync(true);

        data = new EditorData(this);

        guiRenderer.add(new GUIMainMenu(this));
        guiRenderer.add(new GUIScene(this));
        guiRenderer.add(new GUIControlPanel(this));
        guiRenderer.add(new GUIMeshEditor(this));
        guiRenderer.add(new GUIMaterialEditor(this));
        guiRenderer.add(new GUIScriptEditor(this));
        guiRenderer.add(new GUIGameView((this)));
        guiRenderer.add(new GUIViewport(this));
        guiRenderer.add(new GUILogView(this));
        guiRenderer.add(new GUIExplorer(this));
        guiRenderer.add(new GUIProfiling(this));
        guiRenderer.add(new GUIProperties(this));
    }

    public void destroy() {
        guiRenderer.destroy();
        window.destroy();
    }

    public void setup(EditorCallback setupCallback) {
        setupCallback.method(this);
        window.maximize();
        timer.reset();
    }

    public void update() {
        timer.updateDeltaT();

        if (data.gameRunning) {
            physicsEngine.update(scene, timer.getDeltaT());
            scriptEngine.callUpdates(scene);
        }

        window.getContext().clear(false);
        window.getContext().setViewport(window.getSize());

        guiRenderer.render();

        window.swapBuffers();

        GLObject.cleanup();
    }

    public Scene getScene() {
        return scene;
    }

    public boolean setScene(Scene scene) {
        if (Instance.isValid(scene)) {
            this.scene = scene;
            return true;
        }
        return false;
    }

    public void eraseScene() {
        scene = new Scene();
    }
}
