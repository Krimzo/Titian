#pragma once

#include "standard.h"


namespace titian {
	struct TimeBomb
	{
		inline TimeBomb(const auto& callback)
			: m_callback(callback)
			, m_start_time(kl::time::now())
		{}

		inline ~TimeBomb() noexcept
		{
			m_callback(kl::time::calculate(m_start_time, kl::time::now()));
		}

	private:
		const Function<void(float)> m_callback;
		uint64_t m_start_time = 0;
	};
}

namespace titian {
	struct BenchmarkInfo
	{
		const String bench_name;
		float bench_time = 0.0f;
		int bench_draw_calls = 0;

		inline BenchmarkInfo(const StringView& bench_name)
			: bench_name(bench_name)
		{}

		inline void bench_reset() noexcept
		{
			bench_time = 0.0f;
			bench_draw_calls = 0;
		}

		inline TimeBomb bench_time_bomb() noexcept
		{
			return [this](float time) { bench_time = time; };
		}

		inline void bench_add_draw_call() noexcept
		{
			bench_draw_calls += 1;
		}
	};
}
