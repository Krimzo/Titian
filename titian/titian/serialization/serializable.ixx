export module serializable;

export import serializer;

export namespace titian {
    class Serializable
    {
    public:
        Serializable()
        {}

        virtual ~Serializable()
        {}
        
        virtual void serialize(Serializer* serializer) const = 0;
        virtual void deserialize(const Serializer * serializer) = 0;
    };
}
