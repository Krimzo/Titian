#include "state/logger_state.h"


logger_state::logger_state()
{
    this->clear();
}

size_t logger_state::last_log_index() const
{
    return (log_counter_ > 0) ? ((log_counter_ - 1) % this->size()) : 0;
}

size_t logger_state::size() const
{
    return std::vector<log_info>::size();
}

const log_info& logger_state::get(size_t index) const
{
    return std::vector<log_info>::at(index);
}

void logger_state::clear()
{
    std::vector<log_info>::clear();
    this->resize(max_size);
    log_counter_ = 0;
}

void logger_state::log(const log_info& log)
{
    (*this)[log_counter_++ % this->size()] = log;
}
