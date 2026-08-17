#pragma once

#include "EVector2Derivable.hpp"
#include "raylib.h"

class Position : public EVector2Derivable<Position> {
    using EVector2Derivable::EVector2Derivable;
};
