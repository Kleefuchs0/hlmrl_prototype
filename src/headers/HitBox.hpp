#pragma once

#include "Size.hpp"
#include "Position.hpp"
#include <raylib.h>

struct Hitbox {
    Size size;
    Color color;
    Rectangle getRectangle(Position playerPos) {
    return {
        playerPos.x - size.x / 2,
        playerPos.y - size.y / 2,
        size.x,
        size.y
    };
}
};