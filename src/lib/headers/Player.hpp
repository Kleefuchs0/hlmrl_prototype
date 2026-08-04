#pragma once

#include "DeltaSpeed.hpp"
#include "Position.hpp"
#include "BodySize.hpp"
#include "Acceleration.hpp"

struct Player {
    Acceleration movementSpeed;
    Position pos;
    DeltaSpeed deltaSpeed;
    BodySize size;
    float hitBoxRadius;
    float rotation;
    float circularHitBoxRadius;
};
