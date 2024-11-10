#include "titian.h"


titian::TimeBomb::TimeBomb( Func<void( float )> callback )
    : m_callback( std::move( callback ) )
    , m_start_time( kl::time::now() )
{}

titian::TimeBomb::~TimeBomb()
{
    m_callback( kl::time::elapsed( m_start_time ) );
}

titian::BenchmarkInfo::BenchmarkInfo( StringRef const& bench_name )
    : bench_name( bench_name )
{}

void titian::BenchmarkInfo::bench_reset()
{
    bench_time = 0.0f;
    bench_draw_calls = 0;
}

titian::TimeBomb titian::BenchmarkInfo::bench_time_bomb()
{
    return TimeBomb{ [this]( float time ) { bench_time = time; } };
}

void titian::BenchmarkInfo::bench_add_draw_call()
{
    bench_draw_calls += 1;
}
