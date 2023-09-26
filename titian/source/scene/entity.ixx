export module entity;

export import unique;

export namespace titian {
    class Entity : public Unique
    {
    public:
        Entity()
        {}

        ~Entity() override
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
