#pragma once

#include "Map.hpp"
#include "MapMutex.hpp"
#include "TickedFunction.hpp"
#include "constants.hpp"
#include <entt/entt.hpp>
#include <map>
#include <shared_mutex>

struct GameData {
    MapMutex mapMutex;
    Map<DEFAULT_MAP_WIDTH, DEFAULT_MAP_HEIGHT, DEFAULT_TILE_SIZE> map;
    std::map<std::string, TickedFunction> tickedFunctions;
    std::shared_mutex worldSizeMutex;
    int worldWidth = 0;
    int worldHeight = 0;
    GameData(int worldWidth, int worldHeight) : worldWidth(worldWidth), worldHeight(worldHeight) {
    }

    std::shared_mutex registryMutex;
    entt::registry registry;
    std::shared_mutex runningMutex;
    bool running = true;
};
