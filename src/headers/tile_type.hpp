#pragma once

#include "Value.hpp"
#include <cstdint>

#define TILE_TYPE_SECTION_START_EMPTY_DECORATOR 0
#define TILE_TYPE_SECTION_START_FLOORS 1000
#define TILE_TYPE_SECTION_START_WALLS 2000

enum class tile_type : uint16_t {
    EMPTY = TILE_TYPE_SECTION_START_EMPTY_DECORATOR,
    FLOOR = TILE_TYPE_SECTION_START_FLOORS,
    WALL = TILE_TYPE_SECTION_START_WALLS,
};

class TileType : public Value<TileType, tile_type> {
    using Value::Value;
};
