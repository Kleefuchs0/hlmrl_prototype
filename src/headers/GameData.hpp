#pragma once

#include "Player.hpp"
#include "raylib.h"
#include <entt/entt.hpp>

struct GameData {
    int worldWidth;
    int worldHeight;
    entt::registry registry;
    Camera2D cam;
    Player player;
    GameData(const Camera2D cam, const int worldWidth, const int worldHeight) : worldWidth(worldWidth), worldHeight(worldHeight), cam(cam) {
    }
};
