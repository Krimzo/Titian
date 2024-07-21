#pragma once

#include "application/log_info.h"


namespace titian {
	class Logger
	{
		static bool m_is_ready;

	public:
		static List<LogInfo> logs;
		static uint64_t last_log_index;

		static void set_ready();

		template<typename... Args>
		static void log(const Args&... objects)
		{
			// Add to logs
			const LogInfo& info = logs.emplace_front(objects...);
			last_log_index += 1;

			// Adjust size if needed
			while (logs.size() > 250) {
				logs.pop_back();
			}

			// Print if displaying is not ready
			if (!m_is_ready) {
				kl::print("[", info.date, "]: ", info.message);
			}
		}
	};
}
