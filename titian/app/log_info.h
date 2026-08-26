#pragma once

#include "globals.h"

namespace titian
{
struct LogInfo
{
    String date = kl::format(kl::Date());
    String message;

    LogInfo();
    LogInfo(StringRef const& message);

    template <typename... Args> LogInfo(Args&... objects) : message(kl::format(objects...))
    {
    }
};
} // namespace titian
