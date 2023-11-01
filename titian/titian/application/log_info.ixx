export module log_info;

export import klib;

export namespace titian {
	class LogInfo
    {
    public:
        std::string date = kl::format(kl::Date());
        std::string message = {};

        LogInfo()
        {}

        LogInfo(const char* message)
            : message(message)
        {}

        LogInfo(const std::string& message)
            : message(message)
        {}

        template<typename... Args>
        LogInfo(const Args&... objects)
            : message(kl::format(objects...))
        {}
	};
}
