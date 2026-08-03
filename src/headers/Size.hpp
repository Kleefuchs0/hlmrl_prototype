#pragma once

#include "EVector2Derivable.hpp"
#include "raylib.h"
class Size : public EVector2Derivable<Size> {
    using EVector2Derivable::EVector2Derivable;
};
