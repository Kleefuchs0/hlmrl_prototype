#pragma once

#include <shared_mutex>
class BodyRotationMutex : public std::shared_mutex {
    using std::shared_mutex::shared_mutex;
};
