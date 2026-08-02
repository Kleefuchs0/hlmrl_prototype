#pragma once

#include "Value.hpp"
#include "raylib.h"

class Position : public Value<Position, Vector2> {
    using Value::Value;
};
