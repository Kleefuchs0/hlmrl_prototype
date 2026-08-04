#pragma once

#include "Value.hpp"
template <typename Derived>
class Radius : public Value<Derived, float> {
    using Value<Derived, float>::Value;
};
