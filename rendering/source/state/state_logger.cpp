#include "state/state_logger.h"


state_logger::state_logger()
{
    this->resize(max_size);
}

state_logger::~state_logger()
{}

size_t state_logger::get_last_log_index() const
{
    return (log_counter_ - 1) % this->size();
}

size_t state_logger::get_next_log_index() const
{
    return log_counter_ % this->size();
}

size_t state_logger::get_max_size_number_of_digits() const
{
    return (size_t) log10(max_size) + 1;
}
