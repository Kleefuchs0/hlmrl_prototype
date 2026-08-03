#pragma once

#include "Position.hpp"
#include "Size.hpp"
#include "MovementSpeed.hpp"
#include "HitBox.hpp"

struct Player {
    MovementSpeed speed;
    Position pos;
    Size size;
    float rotation;
    Hitbox hitbox;
};
