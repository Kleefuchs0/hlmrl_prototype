#pragma once

#include "Camera2DMutex.hpp"
#include "Map.hpp"
#include "MapMutex.hpp"
#include "TickedFunction.hpp"
#include "constants.hpp"
#include "raylib.h"
#include <cstdint>
#include <entt/entt.hpp>
#include <map>
#include <shared_mutex>

struct GameData {
    RenderTexture2D renderTexture;
    Color backgroundColor = BLUE;
    std::shared_mutex worldSizeMutex;
    int worldWidth = 0;
    int worldHeight = 0;
    double tickClock = 0.0;
    uint_fast64_t tick = 0;
    uint_fast16_t tickRate = 60;
    Color screenTint = WHITE;
    MapMutex mapMutex;
    Map<DEFAULT_MAP_WIDTH, DEFAULT_MAP_HEIGHT, DEFAULT_TILE_SIZE> map;
    std::map<std::string, TickedFunction> tickedFunctions;
    entt::registry registry;
    std::shared_mutex runningMutex;
    bool running = true;
    Camera2DMutex camMutex;
    Camera2D cam;
    GameData(const Camera2D cam, const int worldWidth, const int worldHeight) : worldWidth(worldWidth), worldHeight(worldHeight), cam(cam) {
    }
};
