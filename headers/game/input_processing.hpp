#pragma once

#include "game/InputData.hpp"
#include "lib/DebugConfiguration.hpp"
#include "lib/GameData.hpp"
namespace game {
    namespace input {
        void process(GameData &gameData, const DebugConfiguration &DebugConfiguration, const InputData &inputData, const float deltaTime);
    }
}
