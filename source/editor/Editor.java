package editor;

import glparts.Disposable;
import gui.GUIRenderer;
import gui.sections.*;
import math.Float2;
import math.Float4;
import math.Int2;
import physics.Physics;
import renderer.Renderer;
import scene.Scene;
import utility.Timer;
import window.GLContext;
import window.Window;

import java.awt.*;
import java.util.HashMap;
import java.util.Map;

public class Editor implements Disposable {
    public static boolean DEBUG = false;
    public final Map<String, Object> savedData = new HashMap<>();

    public Window window = new Window(new Int2(1600, 900), "Titian Creator", true);
    public GLContext context = window.getContext();
    public Timer timer = new Timer();

    public Renderer renderer = new Renderer(context, window.getSize());
    public GUIRenderer guiRenderer = new GUIRenderer(window);
    public Physics physics = new Physics();

    private Scene scene = null;

    public Editor() throws Exception {
        window.setIcon("resource/textures/icons/titian.png");
        window.setVSync(true);

        context.setDepthTest(true);
        renderer.camera.setDefaultMovement(window, timer);

        guiRenderer.add(new GUIMainMenu(this));
        guiRenderer.add(new GUIScene(this));
        guiRenderer.add(new GUIMeshEditor(this));
        guiRenderer.add(new GUIMaterialEditor(this));
        guiRenderer.add(new GUIControlPanel(this));
        guiRenderer.add(new GUIViewport(this));
        guiRenderer.add(new GUIExplorer(this));
        guiRenderer.add(new GUILogView(this));
        guiRenderer.add(new GUIProperties(this));

        savedData.put("WireframeState", false);
        savedData.put("ViewportPosition", new Int2());
        savedData.put("ViewportSize", new Int2());
        savedData.put("OutlineColor", new Color(0xDA7315));
    }

    public void setup(EditorCallback editorCallback) throws Exception {
        editorCallback.method(this);
        window.maximize();
        timer.reset();
    }

    public boolean process() {
        return window.process();
    }

    public void update(EditorCallback editorCallback) throws Exception {
        timer.updateDeltaT();

        physics.update(scene, timer.getDeltaT());
        editorCallback.method(this);

        context.clear(false);

        if (scene != null) {
            boolean wireframeState = (boolean) savedData.get("WireframeState");
            Int2 viewportSize = (Int2) savedData.get("ViewportSize");
            Color outlineColor = (Color) savedData.get("OutlineColor");
            int selectedIndex = scene.indexOf(scene.selectedEntity);

            context.setViewport(viewportSize);
            renderer.updateSize(viewportSize);

            context.setWireframe(wireframeState);
            renderer.renderIndices(scene);
            renderer.renderScene(scene);
            context.setWireframe(false);

            if (selectedIndex >= 0) {
                renderer.renderOutline(new Float2(viewportSize), new Float4(outlineColor), selectedIndex);
            }
        }

        context.setViewport(window.getSize());
        guiRenderer.render();

        window.swapBuffers();
    }

    @Override
    public void dispose() {
        guiRenderer.dispose();
        window.dispose();
    }

    public Scene getScene() {
        return scene;
    }

    public void setScene(Scene scene) {
        if (this.scene != null) {
            this.scene.dispose();
        }
        this.scene = scene;
    }
}
