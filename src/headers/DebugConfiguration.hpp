#pragma once

#include "LogLevel.hpp"

struct DebugConfiguration {
    LogLevel logLevel = LogLevel::DEBUG;
    bool drawFPS = true;
};
