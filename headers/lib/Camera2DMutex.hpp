#pragma once

#include <shared_mutex>
class Camera2DMutex : public std::shared_mutex {
    using std::shared_mutex::shared_mutex;
};
