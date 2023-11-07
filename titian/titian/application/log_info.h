#pragma once

#include "klibrary.h"


namespace titian {
	class LogInfo
    {
    public:
        std::string date = kl::format(kl::Date());
        std::string message = {};

        LogInfo();
        LogInfo(const char* message);
        LogInfo(const std::string& message);

        template<typename... Args>
        LogInfo(const Args&... objects)
            : message(kl::format(objects...))
        {}
	};
}
