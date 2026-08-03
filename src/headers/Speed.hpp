#pragma once

#include "Value.hpp"

class Speed : public Value<Speed, float> {
    using Value::value;
};
