package renderer.abs;

import glparts.Shaders;

public interface Renderable {
    void gameRender(Shaders shaders);
    void editorRender(Shaders shaders);
    void indexRender(Shaders shaders);
}
