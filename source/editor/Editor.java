package editor;

import gui.GUIRenderer;
import gui.sections.*;
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

public class Editor {
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
        window.setOnResize(newSize -> {
            renderer.frameBuffer.updateSize(newSize);
            context.setViewport(newSize);
        });
        window.setIcon("resource/textures/icons/k.png");
        window.setVSync(true);

        context.setDepthTest(true);
        context.setClearColor(new Float4(new Color(0x323232)));

        renderer.camera.setDefaultMovement(window, timer);

        guiRenderer.add(new GUIMainMenu(this));
        guiRenderer.add(new GUIScene(this));
        guiRenderer.add(new GUIGamePanel(this));
        guiRenderer.add(new GUIViewport(this));
        guiRenderer.add(new GUIExplorer(this));
        guiRenderer.add(new GUILogView(this));
        guiRenderer.add(new GUIProperties(this));

        savedData.put("WireframeState", false);
        savedData.put("ViewportSize", new Int2());
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

        physics.update(timer.getDeltaT());
        editorCallback.method(this);

        context.clear(false);

        Int2 viewportSize = (Int2) savedData.get("ViewportSize");
        context.setViewport(viewportSize);
        renderer.updateSize(viewportSize);
        renderer.render();

        context.setViewport(window.getSize());
        guiRenderer.render();

        window.swapBuffers();
    }

    public void destroy() {
        guiRenderer.destroy();
        window.destroy();
    }

    public Scene getScene() {
        return scene;
    }

    public void setScene(Scene scene) {
        renderer.remove(this.scene);
        physics.remove(this.scene);

        if (this.scene != null) {
            this.scene.destroy();
        }

        if (scene != null) {
            renderer.add(scene);
            physics.add(scene);
        }

        this.scene = scene;
    }
}
