#pragma once

#include <shared_mutex>
class PositionMutex : public std::shared_mutex {
    using std::shared_mutex::shared_mutex;
};
