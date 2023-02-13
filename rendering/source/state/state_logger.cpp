#include "state/state_logger.h"


state_logger::state_logger()
{
    this->clear();
}

state_logger::~state_logger()
{}

size_t state_logger::last_log_index() const
{
    return (log_counter_ > 0) ? ((log_counter_ - 1) % this->size()) : 0;
}

size_t state_logger::size() const
{
    return std::vector<log_info>::size();
}

const log_info& state_logger::get(size_t index) const
{
    return std::vector<log_info>::at(index);
}

void state_logger::clear()
{
    std::vector<log_info>::clear();
    this->resize(max_size);
    log_counter_ = 0;
}

void state_logger::log(const log_info& log)
{
    (*this)[log_counter_++ % this->size()] = log;
}
