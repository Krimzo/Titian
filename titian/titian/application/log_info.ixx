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

        LogInfo(const std::string_view& message)
        {
            this->message = message;
        }

        template<typename T>
        LogInfo(const T& object)
        {
            message = kl::format(object);
        }
	};
}
