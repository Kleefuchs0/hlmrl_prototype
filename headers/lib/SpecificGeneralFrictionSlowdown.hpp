#pragma once

#include "FrictionSlowdown.hpp"
class SpecificGeneralFrictionSlowdown : public FrictionSlowdown<SpecificGeneralFrictionSlowdown> {
    using FrictionSlowdown<SpecificGeneralFrictionSlowdown>::FrictionSlowdown;
};
