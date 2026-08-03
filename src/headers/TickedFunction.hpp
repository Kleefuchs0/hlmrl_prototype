#pragma once

#include <cstdint>

class GameData;

class TickedFunction {
    public:
     uint_fast16_t tickGoal = 1;
     void (*function)(GameData&) = nullptr;
     TickedFunction() = default;
     TickedFunction(uint_fast16_t tickGoal, void (*function) (GameData&));
};
