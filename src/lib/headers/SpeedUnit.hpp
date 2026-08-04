#pragma once

#include "Value.hpp"

template <typename Derived>
class SpeedUnit : public Value<Derived, float> {
    using Value<Derived, float>::Value;
};
