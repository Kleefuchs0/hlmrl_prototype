#include "lib/TickedFunction.hpp"
#include "lib/DebugConfiguration.hpp"
#include <cassert>

TickedFunction::TickedFunction(uint_fast16_t tickGoal, void (*function) (GameData&, DebugConfiguration&)) {
    assert(tickGoal > 0);
    this->tickGoal = tickGoal;
    this->function = function;
}

