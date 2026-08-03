#pragma once

#include "Value.hpp"
#include <cstdint>

enum class tile_type : uint8_t {
    EMPTY,
    WALL,
    FLOOR
};

class TileType : public Value<TileType, tile_type> {
    using Value::Value;
};
