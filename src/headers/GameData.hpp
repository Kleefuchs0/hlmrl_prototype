#pragma once

#include "Map.hpp"
#include "Player.hpp"
#include "Hitbox.hpp"
#include "TickedFunction.hpp"
#include "constants.hpp"
#include "raylib.h"
#include <cstdint>
#include <entt/entt.hpp>
#include <map>

struct GameData {
    RenderTexture2D renderTexture;
    int worldWidth = 0;
    int worldHeight = 0;
    double tickClock = 0.0;
    uint_fast64_t tick = 0;
    uint_fast16_t tickRate = 60;
    Color screenTint = WHITE;
    Map<DEFAULT_MAP_WIDTH, DEFAULT_MAP_HEIGHT> map;
    std::map<std::string, TickedFunction> tickedFunctions;
    entt::registry registry;
    Camera2D cam;
    Hitbox hitbox;
    Player player;
    GameData(const Camera2D cam, const int worldWidth, const int worldHeight) : worldWidth(worldWidth), worldHeight(worldHeight), cam(cam) {
    }
};
