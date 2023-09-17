export module logger_state;

export import basic_state;

export class LogInfo
{
public:
    std::string date = kl::format(kl::Date());
    std::string message = {};

    LogInfo()
    {}

    template<typename T>
    LogInfo(const T& object)
        : message(kl::format(object))
    {}
};

export class LoggerState : public BasicState
{
    std::vector<LogInfo> m_logs = {};
    size_t m_log_counter = 0;

public:
    size_t max_size = 100;

    LoggerState()
    {
        this->clear();
    }

    LoggerState(const LoggerState&) = delete;
    LoggerState(const LoggerState&&) = delete;

    void operator=(const LoggerState&) = delete;
    void operator=(const LoggerState&&) = delete;

    size_t last_log_index() const
    {
        return (m_log_counter > 0) ? ((m_log_counter - 1) % m_logs.size()) : 0;
    }

    size_t size() const
    {
        return m_logs.size();
    }

    const LogInfo& get(size_t index) const
    {
        return m_logs[index];
    }

    void log(const LogInfo& log)
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
