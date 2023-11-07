#include "main.h"


titian::LogInfo::LogInfo()
{}

titian::LogInfo::LogInfo(const char* message)
    : message(message)
{}

titian::LogInfo::LogInfo(const std::string& message)
    : message(message)
{}
