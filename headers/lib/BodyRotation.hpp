#pragma once

#include "Rotation.hpp"

class BodyRotation : public Rotation<BodyRotation> {
    using Rotation<BodyRotation>::Rotation;
};
