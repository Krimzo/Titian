export module scene;

export import entity;

export namespace titian {
    class Scene : public Unique
    {
        std::vector<kl::Object<kl::Entity>> m_entities = {};
        
    public:
        Scene()
        {}

        ~Scene() override
        {}

        void serialize(kl::File* file) const override
        {
            Unique::serialize(file);
        }
        
        void deserialize(const kl::File* file) override
        {
            Unique::deserialize(file);
        }
    };
}
