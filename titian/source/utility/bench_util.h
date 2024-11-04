#pragma once

#include "standard.h"


namespace titian {
	struct TimeBomb
	{
		TimeBomb(Func<void(float)> callback);
		~TimeBomb() noexcept;

	private:
		const Func<void(float)> m_callback;
		const uint64_t m_start_time;
	};
}

namespace titian {
	struct BenchmarkInfo
	{
		const String bench_name;
		float bench_time = 0.0f;
		int bench_draw_calls = 0;

		BenchmarkInfo(const StringRef& bench_name);

		void bench_reset();
		TimeBomb bench_time_bomb();
		void bench_add_draw_call();
	};
}
