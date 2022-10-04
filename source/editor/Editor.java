package editor;

import callback.EditorCallback;
import glparts.abs.Disposable;
import gui.GUIRenderer;
import gui.section.*;
import math.Float2;
import math.Float4;
import math.Int2;
import physics.Physics;
import renderer.Renderer;
import scene.Scene;
import script.Scripter;
import utility.Timer;
import window.Window;

import java.awt.*;
import java.util.HashMap;
import java.util.Map;
import logging.Logger;

public class Editor implements Disposable {
    public final Map<String, Object> savedData = new HashMap<>();

    public Window window = new Window(new Int2(1600, 900), "Titian Creator", true);
    public Timer timer = new Timer();

    public Renderer renderer = new Renderer(window.getContext(), window.getSize());
    public GUIRenderer guiRenderer = new GUIRenderer(window);
    public Physics physics = new Physics();

    public Logger logger = new Logger();
    public Scripter scripter = new Scripter();

    public Scene scene = null;

    public Editor() throws Exception {
        window.setIcon("resource/textures/icons/titian.png");
        window.setVSync(true);

        window.getContext().setDepthTest(true);
        renderer.camera.setDefaultMovement(window, timer);

        guiRenderer.add(new GUIMainMenu(this));
        guiRenderer.add(new GUIScene(this));
        guiRenderer.add(new GUIControlPanel(this));
        guiRenderer.add(new GUIMeshEditor(this));
        guiRenderer.add(new GUIMaterialEditor(this));
        guiRenderer.add(new GUIScriptEditor(this));
        guiRenderer.add(new GUIViewport(this));
        guiRenderer.add(new GUIExplorer(this));
        guiRenderer.add(new GUILogView(this));
        guiRenderer.add(new GUIProperties(this));

        savedData.put("SpecialColor", new Color(190, 115, 10));
        savedData.put("GameRunning", false);
        savedData.put("WireframeState", false);
        savedData.put("ViewportPosition", new Int2());
        savedData.put("ViewportSize", new Int2());
        savedData.put("GizmoOperation", 0);
    }

    public void setup(EditorCallback setupCallback) {
        setupCallback.method(this);
        window.maximize();
        timer.reset();
    }

    public boolean process() {
        return window.process();
    }

    public void update() {
        timer.updateDeltaT();

        if ((boolean) savedData.get("GameRunning")) {
            physics.update(scene, timer.getDeltaT());
            scripter.callUpdates(scene);
        }

        window.getContext().clear(false);

        if (scene != null) {
            boolean wireframeState = (boolean) savedData.get("WireframeState");
            Int2 viewportSize = (Int2) savedData.get("ViewportSize");
            Color outlineColor = (Color) savedData.get("SpecialColor");
            int selectedIndex = scene.indexOf(scene.selectedEntity);

            window.getContext().setViewport(viewportSize);
            renderer.resize(viewportSize);

            window.getContext().setWireframe(wireframeState);
            renderer.renderIndices(scene);
            renderer.renderScene(scene);
            window.getContext().setWireframe(false);

            if (selectedIndex >= 0) {
                renderer.renderOutline(new Float2(viewportSize), new Float4(outlineColor), selectedIndex);
            }
        }

        window.getContext().setViewport(window.getSize());
        guiRenderer.render();

        window.swapBuffers();
    }

    @Override
    public void dispose() {
        renderer.dispose();
        guiRenderer.dispose();
        window.dispose();
    }

    public void destroyCurrentScene() {
        if (scene != null) {
            scene.dispose();
            scene = null;
        }
    }
}
