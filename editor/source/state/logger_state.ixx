export module logger_state;

export import basic_state;

export struct log_info
{
    std::string date = kl::format(kl::date());
    std::string message = {};

    log_info()
    {}

    template<typename T>
    log_info(const T& object)
        : message(kl::format(object))
    {}
};

export class logger_state : public BasicState
{
    std::vector<log_info> m_logs = {};
    size_t m_log_counter = 0;

public:
    size_t max_size = 100;

    logger_state()
    {
        this->clear();
    }

    logger_state(const logger_state&) = delete;
    logger_state(const logger_state&&) = delete;

    void operator=(const logger_state&) = delete;
    void operator=(const logger_state&&) = delete;

    size_t last_log_index() const
    {
        return (m_log_counter > 0) ? ((m_log_counter - 1) % m_logs.size()) : 0;
    }

    size_t size() const
    {
        return m_logs.size();
    }

    const log_info& get(size_t index) const
    {
        return m_logs[index];
    }

    void log(const log_info& log)
    {
        m_logs[m_log_counter++ % m_logs.size()] = log;
    }

    void clear()
    {
        m_logs.clear();
        m_logs.resize(max_size);
        m_log_counter = 0;
    }
};
