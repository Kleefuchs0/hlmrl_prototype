#pragma once

#include "Position.hpp"
#include "Size.hpp"
#include "Speed.hpp"

struct Player {
    Speed speed;
    Position pos;
    Size size;
    float rotation;
};
