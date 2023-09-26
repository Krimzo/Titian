export module editor_layer;

export import game_layer;

export namespace titian {
    class EditorLayer : public Layer
    {
        kl::Object<GameLayer> m_game_layer = nullptr;

    public:
        EditorLayer(const kl::Object<GameLayer>& game_layer)
            : m_game_layer(game_layer)
        {}

        ~EditorLayer() override
        {}

        bool update() override
        {
            return true;
        }
    };
}
