#pragma once

#include "Position.hpp"
#include "Size.hpp"

struct HitBoxRectangle {
    Position pos;
    Size size;
    float rotation = 0;
};

bool GetCollisionHitBoxes(const HitBoxRectangle hbr1, const HitBoxRectangle hbr2);
