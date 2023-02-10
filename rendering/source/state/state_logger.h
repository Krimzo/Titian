#pragma once

#include "klib.h"


class state_logger : public std::vector<std::string>
{
    size_t log_counter_ = 1;

    size_t get_next_log_index() const;
    size_t get_max_size_number_of_digits() const;

public:
    size_t max_size = 100;

    state_logger();
    ~state_logger();

    state_logger(const state_logger&) = delete;
    state_logger(const state_logger&&) = delete;

    void operator=(const state_logger&) = delete;
    void operator=(const state_logger&&) = delete;

    size_t get_last_log_index() const;

    template<typename T>
    void log(const T& object)
    {
        this->resize(max_size);
        const size_t log_index = get_next_log_index();
        log_counter_ += 1;

        std::stringstream log_stream = {};
        log_stream << std::setw(get_max_size_number_of_digits()) << (log_index + 1);
        log_stream << ". [" << kl::date() << "]: " << object;
        (*this)[log_index] = log_stream.str();
    }
};
