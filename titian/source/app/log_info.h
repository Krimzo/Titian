#pragma once

#include "standard.h"


namespace titian {
	struct LogInfo
    {
        String date = kl::format(kl::Date());
        String message;

        LogInfo();
        LogInfo(const StringRef& message);

        template<typename... Args>
        LogInfo(const Args&... objects)
            : message(kl::format(objects...))
        {}
	};
}
