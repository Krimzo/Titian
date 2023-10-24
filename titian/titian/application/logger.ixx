export module logger;

export import log_info;

export namespace titian {
	class Logger
	{
	public:
		static constexpr uint64_t max_size = 250;
		static std::list<LogInfo> logs;
		static uint64_t last_log_index;

		static void log(const std::string& message)
		{
			logs.emplace_front(message);
			while (logs.size() > max_size) {
				logs.pop_back();
			}
			last_log_index += 1;
		}

		template<typename T>
		static void log_object(const T& object)
		{
			log(kl::format(object));
		}
	};
}

std::list<titian::LogInfo> titian::Logger::logs = {};
uint64_t titian::Logger::last_log_index = 0;
