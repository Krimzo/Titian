export module editor_layer;

export import game_layer;

export namespace titian {
    class EditorLayer : public Layer
    {
    public:
        kl::Object<GameLayer> game_layer = nullptr;

        kl::Object<Entity> selected_entity = nullptr;

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
