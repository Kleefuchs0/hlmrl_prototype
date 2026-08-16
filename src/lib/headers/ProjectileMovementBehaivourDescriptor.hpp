#pragma once

#include "Acceleration.hpp"
#include "SpecificGeneralFrictionSlowdown.hpp"

struct ProjectileMovementBehaivourDescriptor {
    Acceleration initialAcceleration = 0;
    SpecificGeneralFrictionSlowdown frictionSlowdown = 0.0f;
};
