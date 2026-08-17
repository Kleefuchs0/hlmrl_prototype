#pragma once

#include "BodyRotation.hpp"
#include "BodyRotationMutex.hpp"
#include "DebugConfiguration.hpp"
#include "GameData.hpp"
#include "PickUpMarker.hpp"
#include "PlayerMarker.hpp"
#include "Position.hpp"
#include "PositionMutex.hpp"
#include <chrono>
#include <cstdint>
#include <thread>
#include <mutex>
#include "game_entity_movement_update.hpp"
#include "game_entity_update.hpp"
#include "game_entity_update.hpp"

class PhysicsManagementSettings {
    public:
        std::shared_mutex tickRateMutex;
        uint32_t tickRate = 320;
        PhysicsManagementSettings(uint32_t tickRate) : tickRate(tickRate) {
        }
};

void players_cam_update(GameData &gameData, [[maybe_unused]] DebugConfiguration &debugConfiguration) {
    std::shared_lock registryLock(gameData.registryMutex);
    auto playerView = gameData.registry.view<PlayerMarker, Position, PositionMutex>();
    for (const entt::entity &player : playerView) {
        const auto &[position, positionMutex] = gameData.registry.get<Position, PositionMutex>(player);
        std::shared_lock positionLock(positionMutex);
        std::unique_lock camLock(gameData.camMutex);
        gameData.cam.target = position;
    }
}

void pickup_update(BodyRotation &bodyRotation, BodyRotationMutex &bodyRotationMutex, const float frameTime) {
    std::unique_lock bodyRotationLock(bodyRotationMutex);
    bodyRotation += 100.0f * frameTime;
}

void pickups_update(GameData &gameData, [[maybe_unused]] DebugConfiguration &debugConfiguration, const float frameTime) {
    std::shared_lock registryLock(gameData.registryMutex);
    auto weaponView = gameData.registry.view<PickUpMarker, BodyRotation, BodyRotationMutex>();
    for (const entt::entity &weapon : weaponView) {
        const auto &[bodyRotation, bodyRotationMutex] = gameData.registry.get<BodyRotation, BodyRotationMutex>(weapon);
        pickup_update(bodyRotation, bodyRotationMutex, frameTime);
    }
}

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
            pickups_update(*gameData, *debugConfiguration, tickTime);
            game::loop::update::entities_friction(*gameData, *debugConfiguration, tickTime);
            game::loop::update::entities_movement_update(*gameData, *debugConfiguration, tickTime);
            players_cam_update(*gameData, *debugConfiguration);
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

