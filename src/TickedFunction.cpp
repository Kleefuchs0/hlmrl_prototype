#include "TickedFunction.hpp"

TickedFunction::TickedFunction(uint_fast16_t tickGoal, void (*function) (GameData&)) {
    this->tickGoal = tickGoal;
    this->function = function;
}

