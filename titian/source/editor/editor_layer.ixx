export module editor_layer;

export import game_layer;

export namespace titian {
    class EditorLayer : public Layer
    {
    public:
        kl::Object<GameLayer> game_layer = nullptr;

        bool v_sync = true;
        bool render_wireframe = false;
        bool render_colliders = true;

        EditorLayer(kl::Object<GameLayer>& game_layer)
        {
            this->game_layer = game_layer;
        }

        ~EditorLayer() override
        {}

        bool update() override
        {
            return true;
        }
    };
}
