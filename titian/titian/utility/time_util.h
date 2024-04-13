#pragma once

#include "klibrary.h"


namespace titian {
	class TimeBomb
	{
		const std::function<void(float)> m_callback{};
		uint64_t m_start_time = 0;

	public:
		template<typename T = void(float)>
		TimeBomb(const T& callback)
			: m_callback(callback)
		{
			m_start_time = kl::time::now();
		}

		inline ~TimeBomb() noexcept
		{
			const uint64_t end_time = kl::time::now();
			const float elapsed_time = kl::time::calculate(m_start_time, end_time);
			m_callback(elapsed_time);
		}
	};
}

namespace titian {
	class BenchmarkInfo
	{
	public:
		const std::string benchmark_name;
		float benchmark_time = 0.0f;

		inline BenchmarkInfo(const std::string& benchmark_name)
			: benchmark_name(benchmark_name)
		{}

		inline TimeBomb time_it() noexcept
		{
			return TimeBomb([&](const float time) { this->benchmark_time = time; });
		}
	};
}