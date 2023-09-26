export module serializable;

export import klib;

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
        
        virtual void serialize(kl::File* file) const
        {}
        
        virtual void deserialize(const kl::File* file)
        {
            m_was_deserialized = true;
        }
    };
}
