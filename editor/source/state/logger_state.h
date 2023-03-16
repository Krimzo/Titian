#pragma once

#include "klib.h"


struct log_info
{
    std::string date = kl::format(kl::date());
    std::string message = {};

    log_info() = default;

    template<typename T>
    log_info(const T& object)
        : message(kl::format(object))
    {}
};

class logger_state : private std::vector<log_info>
{
    size_t log_counter_ = 0;

public:
    size_t max_size = 100;

    logger_state();
    virtual ~logger_state();

    logger_state(const logger_state&) = delete;
    logger_state(const logger_state&&) = delete;

    void operator=(const logger_state&) = delete;
    void operator=(const logger_state&&) = delete;

    size_t last_log_index() const;

    size_t size() const;

    const log_info& get(size_t index) const;

    void log(const log_info& log);

    void clear();
};
