#pragma once

#include "Value.hpp"
#include <cstdint>

#define TILE_TYPE_SECTION_START_EMPTY_DECORATOR static_cast<tile_type>(0)
#define TILE_TYPE_SECTION_START_FLOORS static_cast<tile_type> (1000)
#define TILE_TYPE_SECTION_START_WALLS static_cast<tile_type> (2000)

enum class tile_type : uint16_t {
    EMPTY = 0,
    FLOOR = 1000,
    WALL = 2000,
};

class TileType : public Value<TileType, tile_type> {
    using Value::Value;
};
