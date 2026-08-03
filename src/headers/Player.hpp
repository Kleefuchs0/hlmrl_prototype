#pragma once

#include "DeltaSpeed.hpp"
#include "Position.hpp"
#include "Size.hpp"
#include "MovementSpeed.hpp"

struct Player {
    MovementSpeed movementSpeed;
    Position pos;
    DeltaSpeed deltaSpeed;
    Size size;
    float rotation;
    float circularHitBoxRadius;
};
