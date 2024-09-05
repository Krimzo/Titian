#include "titian.h"


// TimeBomb
titian::TimeBomb::TimeBomb(Function<void(float)> callback)
	: m_callback(std::move(callback))
	, m_start_time(kl::time::now())
{}

titian::TimeBomb::~TimeBomb()
{
	m_callback(kl::time::calculate(m_start_time, kl::time::now()));
}

// BenchmarkInfo
titian::BenchmarkInfo::BenchmarkInfo(const StringView& bench_name)
	: bench_name(bench_name)
{}

void titian::BenchmarkInfo::bench_reset()
{
	bench_time = 0.0f;
	bench_draw_calls = 0;
}

titian::TimeBomb titian::BenchmarkInfo::bench_time_bomb()
{
	return TimeBomb{ [this](float time) { bench_time = time; } };
}

void titian::BenchmarkInfo::bench_add_draw_call()
{
	bench_draw_calls += 1;
}
