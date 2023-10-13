export module serializable;

export import serializer;

export namespace titian {
    class Serializable
    {
        bool m_was_deserialized = false;
        
    public:
        Serializable()
        {}

        virtual ~Serializable()
        {}

        bool was_deserialized() const
        {
            return m_was_deserialized;
        }
        
        virtual void serialize(Serializer* serializer) const
        {}
        
        virtual void deserialize(const Serializer* serializer)
        {
            m_was_deserialized = true;
        }
    };
}
