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
        
        void serialize(kl::File* file) const override
        {
            Serializable::serialize(file);
            file->write(m_id);
        }
        
        void deserialize(const kl::File* file) override
        {
            Serializable::deserialize(file);
            file->read(m_id);
        }

        IDType id() const
        {
            return m_id;
        }
    };
}
