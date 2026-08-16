#pragma once

#include "Value.hpp"

template <typename Derived>
class FrictionSlowdown : public Value<Derived, float> {
    using Value<Derived, float>::Value;
};
