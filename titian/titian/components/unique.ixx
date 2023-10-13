export module unique;

export import serializable;

export namespace titian {
    class Unique : public Serializable
    {
    public:
        using IDType = uint64_t;
        
    private:
        IDType m_id = -1;
        
    public:
        Unique()
        {
            static std::atomic<IDType> unique_id_counter = {};
            m_id = ++unique_id_counter;
        }
        
        ~Unique() override
        {}
        
        void serialize(Serializer* serializer) const override
        {
            Serializable::serialize(serializer);

            serializer->write_object(m_id);
        }
        
        void deserialize(const Serializer* serializer) override
        {
            Serializable::deserialize(serializer);

            serializer->read_object(m_id);
        }

        IDType id() const
        {
            return m_id;
        }
    };
}
