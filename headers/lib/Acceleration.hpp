#pragma once

#include "SpeedUnit.hpp"

class Acceleration : public SpeedUnit<Acceleration> {
    using SpeedUnit<Acceleration>::SpeedUnit;
};
