#pragma once

#include "DebugConfiguration.hpp"
#include "GameData.hpp"
#include <thread>

void update(GameData *gameData, DebugConfiguration *debugConfiguration);

class PhysicsManagement {
    private:
        GameData &gameData;
        DebugConfiguration &debugConfiguration;
        std::jthread physicsThread;
    public:
        PhysicsManagement(GameData &gameData, DebugConfiguration &debugConfiguration) : gameData(gameData), debugConfiguration(debugConfiguration), physicsThread(update, &this->gameData, &this->debugConfiguration) {
        }
};

