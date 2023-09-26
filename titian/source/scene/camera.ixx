export module camera;

export import entity;

export namespace titian {
    class Camera : public Entity
    {
    public:
        Camera()
        {}

        ~Camera() override
        {}

        void serialize(kl::File* file) const override
        {
            Entity::serialize(file);
        }

        void deserialize(const kl::File* file) override
        {
            Entity::deserialize(file);
        }
    };
}
