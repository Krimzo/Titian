#pragma once

#include "standard/standard.h"


namespace titian {
	class LogInfo
    {
    public:
        String date = kl::format(kl::Date());
        String message = {};

        LogInfo();
        LogInfo(const char* message);
        LogInfo(const String& message);

        template<typename... Args>
        LogInfo(const Args&... objects)
            : message(kl::format(objects...))
        {}
	};
}
