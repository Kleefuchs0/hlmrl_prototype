#pragma once

#include "Value.hpp"

template <typename Derived>
class Rotation : public Value<Derived, float> {
    using Value<Derived, float>::Value;
};
