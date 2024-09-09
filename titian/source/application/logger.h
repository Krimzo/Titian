#pragma once

#include "application/log_info.h"


namespace titian {
	struct Logger
	{
		static List<LogInfo> logs;
		static uint64_t last_log_index;

		static void set_ready();

		template<typename... Args>
		static void log(const Args&... objects)
		{
			const LogInfo& info = logs.emplace_back(objects...);
			last_log_index += 1;

			while (logs.size() > 250) {
				logs.pop_front();
			}

			if (!m_is_ready) {
				kl::print("[", info.date, "]: ", info.message);
			}
		}

	private:
		static bool m_is_ready;
	};
}
