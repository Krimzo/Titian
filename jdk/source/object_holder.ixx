export module object_holder;

export import java;

export namespace java {
    template<typename T>
    class ObjectHolder
    {
        T m_object = nullptr;

    public:
        // Construct
        ObjectHolder(T object)
            : m_object(object)
        {}

        virtual ~ObjectHolder()
        {
            this->set(nullptr);
        }

        // Copy
        ObjectHolder(const ObjectHolder&) = delete;
        ObjectHolder(const ObjectHolder&&) = delete;

        void operator=(const ObjectHolder&) = delete;
        void operator=(const ObjectHolder&&) = delete;

        // Info
        void set(T object)
        {
            if (m_object) {
                delete_object(m_object);
            }
            m_object = object;
        }

        T get() const
        {
            return m_object;
        }

        operator bool() const
        {
            return (bool) m_object;
        }
    };
}
