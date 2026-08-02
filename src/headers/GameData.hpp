#pragma once

#include "Player.hpp"
#include "entt/entity/fwd.hpp"
#include "raylib.h"
#include <entt/entt.hpp>

struct GameData {
    entt::registry registry;
    Camera2D cam;
    Player player;
    GameData(const Camera2D cam) : cam(cam) {
    }
};
