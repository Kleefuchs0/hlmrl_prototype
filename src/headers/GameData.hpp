#pragma once

#include "Map.hpp"
#include "Player.hpp"
#include "TickedFunction.hpp"
#include "constants.hpp"
#include "raylib.h"
#include <cstdint>
#include <entt/entt.hpp>
#include <map>

struct GameData {
    int worldWidth;
    int worldHeight;
    double tickClock;
    uint_fast64_t tick = 0;
    uint_fast16_t tickRate = 60;
    Map<DEFAULT_MAP_WIDTH, DEFAULT_MAP_HEIGHT> map;
    std::map<std::string, TickedFunction> tickedFunctions;
    entt::registry registry;
    Camera2D cam;
    Player player;
    GameData(const Camera2D cam, const int worldWidth, const int worldHeight) : worldWidth(worldWidth), worldHeight(worldHeight), cam(cam) {
    }
};
