export module java_holder;

export import java;

export template<typename T>
class JavaHolder
{
    T m_object = nullptr;

public:
    // Construct
    JavaHolder(T object)
        : m_object(object)
    {}

    virtual ~JavaHolder()
    {
        this->set(nullptr);
    }

    // Copy
    JavaHolder(const JavaHolder&) = delete;
    JavaHolder(const JavaHolder&&) = delete;

    void operator=(const JavaHolder&) = delete;
    void operator=(const JavaHolder&&) = delete;

    // Info
    void set(T object)
    {
        if (m_object) {
            java::delete_object(m_object);
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
