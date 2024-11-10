#pragma once

#include "standard.h"


namespace titian
{
struct WorkQueue
{
    WorkQueue() = default;

    template<typename T>
    void add_task( T const& task )
    {
        m_queue.push_back( std::async( task ) );
    }

    void finalize();

private:
    Vector<std::future<void>> m_queue;
};
}
