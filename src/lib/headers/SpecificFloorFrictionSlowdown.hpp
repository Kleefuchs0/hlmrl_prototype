#pragma once

#include "FrictionSlowdown.hpp"
#include "Value.hpp"

class SpecificFloorFrictionSlowdown : public FrictionSlowdown<SpecificFloorFrictionSlowdown> {
    using FrictionSlowdown<SpecificFloorFrictionSlowdown>::FrictionSlowdown;
};
