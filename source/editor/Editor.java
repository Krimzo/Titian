package editor;

import callback.EditorCallback;
import camera.PerspectiveCamera;
import glparts.abs.Disposable;
import gui.GUIRenderer;
import gui.section.*;
import logging.Logger;
import math.Int2;
import physics.Physics;
import renderer.EditorRenderer;
import renderer.GameRenderer;
import scene.Scene;
import script.Scripter;
import utility.Timer;
import window.Window;

public class Editor implements Disposable {
    public EditorSharedData data;

    public Window window = new Window(new Int2(1600, 900), "Titian", true);
    public Timer timer = new Timer();

    public final PerspectiveCamera camera = new PerspectiveCamera(null, "Editor Camera", this);
    public EditorRenderer editorRenderer = new EditorRenderer(window.getContext(), window.getSize());
    public GameRenderer gameRenderer = new GameRenderer(window.getContext(), window.getSize());
    public GUIRenderer guiRenderer = new GUIRenderer(window);

    public Physics physics = new Physics();

    public Logger logger = new Logger();
    public Scripter scripter = new Scripter();

    public Scene scene = null;

    public Editor() throws Exception {
        window.setIcon("resource/textures/titian.png");
        window.getContext().setDepthTest(true);
        window.setVSync(true);

        data = new EditorSharedData(this);

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

    public void setup(EditorCallback setupCallback) {
        setupCallback.method(this);
        window.maximize();
        timer.reset();
    }

    public void update() {
        timer.updateDeltaT();

        if (data.gameRunning) {
            physics.update(scene, timer.getDeltaT());
            scripter.callUpdates(scene);
        }

        window.getContext().clear(false);
        window.getContext().setViewport(window.getSize());

        guiRenderer.renderGUI();

        window.swapBuffers();
    }

    @Override
    public void dispose() {
        data.dispose();
        gameRenderer.dispose();
        editorRenderer.dispose();
        guiRenderer.dispose();
        window.dispose();
    }

    public void disposeCurrentScene() {
        if (scene != null) {
            scene.dispose();
            scene = null;
        }
    }
}
