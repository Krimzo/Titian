#include "state/state_logger.h"


state_logger::state_logger()
{
    this->clear();
}

state_logger::~state_logger()
{}

void state_logger::clear()
{
    std::vector<std::string>::clear();
    this->resize(max_size);
    log_counter_ = 0;
}

size_t state_logger::get_last_log_index() const
{
    if (log_counter_ > 0) {
        return (log_counter_ - 1) % this->size();
    }
    return 0;
}

size_t state_logger::get_next_log_index() const
{
    return log_counter_ % this->size();
}

size_t state_logger::get_max_size_number_of_digits() const
{
    return (size_t) log10(max_size) + 1;
}
