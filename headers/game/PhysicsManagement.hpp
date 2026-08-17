#pragma once

#include "game/pickups_update.hpp"
#include "game/player_cam_update.hpp"
#include "lib/DebugConfiguration.hpp"
#include "lib/GameData.hpp"
#include <chrono>
#include <cstdint>
#include <thread>
#include "entity_movement_update.hpp"
#include "entity_fricition_update.hpp"

class PhysicsManagementSettings {
    public:
        std::shared_mutex tickRateMutex;
        uint32_t tickRate = 320;
        PhysicsManagementSettings(uint32_t tickRate) : tickRate(tickRate) {
        }
};

void update(GameData *gameData, DebugConfiguration *debugConfiguration, PhysicsManagementSettings *settings) {
    std::shared_lock runningLock(gameData->runningMutex);
    auto previous = std::chrono::steady_clock::now();

    double tickClock = 0;
    while (gameData->running) {
        auto now = std::chrono::steady_clock::now();

        double frameTime = std::chrono::duration<double>(now - previous).count();

        previous = now;

        runningLock.unlock();

        tickClock += frameTime;
        std::shared_lock tickRateLock(settings->tickRateMutex);
        double tickTime = 1.0 / settings->tickRate;
        tickRateLock.unlock();
        std::this_thread::sleep_for(std::chrono::duration<double>(tickTime - tickClock / 1.2));

        while(tickClock >= tickTime) {
            game::physics::update_pickups(*gameData, *debugConfiguration, tickTime);
            game::physics::update_entities_friction(*gameData, *debugConfiguration, tickTime);
            game::physics::movement::update_entities_movement(*gameData, *debugConfiguration, tickTime);
            game::physics::update_players_cam(*gameData, *debugConfiguration);
            tickClock -= tickTime;
        }

        runningLock.lock();
    }
};

class PhysicsManagement {
    private:
        GameData &gameData;
        DebugConfiguration &debugConfiguration;
        PhysicsManagementSettings settings;
    public:
        PhysicsManagement(GameData &gameData, DebugConfiguration &debugConfiguration, const uint32_t tickRate) : gameData(gameData), debugConfiguration(debugConfiguration), settings(tickRate)  {
        }

        void start() {
            std::jthread thread(update, &gameData, &debugConfiguration, &this->settings);
            thread.detach();
        }
};

