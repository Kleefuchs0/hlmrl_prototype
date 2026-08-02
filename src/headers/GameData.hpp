#pragma once

#include "entt/entity/fwd.hpp"
#include "raylib.h"
#include <entt/entt.hpp>

struct GameData {
    entt::registry registry;
    Camera2D cam;
    GameData(const Camera2D cam) : cam(cam) {
    }
};
