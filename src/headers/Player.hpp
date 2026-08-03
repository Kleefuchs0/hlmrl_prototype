#pragma once

#include "Position.hpp"
#include "Size.hpp"
#include "MovementSpeed.hpp"

struct Player {
    MovementSpeed speed;
    Position pos;
    Size size;
    float hitBoxRadius;
    float rotation;
};
