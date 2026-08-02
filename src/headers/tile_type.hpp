#pragma once

#include "Value.hpp"
#include <cstdint>

class tile_type : public Value<tile_type, uint8_t> {
    using Value::Value;
};
