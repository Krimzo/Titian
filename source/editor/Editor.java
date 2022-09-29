package editor;

import callback.EditorCallback;
import utility.Disposable;
import gui.GUIRenderer;
import gui.sections.*;
import math.Float2;
import math.Float4;
import math.Int2;
import physics.Physics;
import renderer.Renderer;
import scene.Scene;
import utility.Timer;
import window.Window;

import java.awt.*;
import java.util.HashMap;
import java.util.Map;

public class Editor implements Disposable {
    public final Map<String, Object> savedData = new HashMap<>();

    public Window window = new Window(new Int2(1600, 900), "Titian Creator", true);
    public Timer timer = new Timer();

    public Renderer renderer = new Renderer(window.getContext(), window.getSize());
    public GUIRenderer guiRenderer = new GUIRenderer(window);
    public Physics physics = new Physics();

    private Scene scene = null;

    public Editor() throws Exception {
        window.setIcon("resource/textures/icons/titian.png");
        window.setVSync(true);

        window.getContext().setDepthTest(true);
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

        window.getContext().clear(false);

        if (scene != null) {
            boolean wireframeState = (boolean) savedData.get("WireframeState");
            Int2 viewportSize = (Int2) savedData.get("ViewportSize");
            Color outlineColor = (Color) savedData.get("OutlineColor");
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
        guiRenderer.dispose();
        window.dispose();
    }

    public Scene getScene() {
        return scene;
    }

    public void changeScene(Scene scene) {
        if (this.scene != null) {
            this.scene.dispose();
        }
        this.scene = scene;
    }
}
