#pragma once

#include "Acceleration.hpp"
#include "BodyRotation.hpp"
#include "BodyRotationMutex.hpp"
#include "DebugConfiguration.hpp"
#include "GameData.hpp"
#include "HitBoxRadius.hpp"
#include "PickUpMarker.hpp"
#include "PlayerMarker.hpp"
#include "Position.hpp"
#include "PositionMutex.hpp"
#include "SpeedVector.hpp"
#include "SpeedVectorMutex.hpp"
#include <chrono>
#include <thread>
#include "entity_try_move.hpp"
#include "game_entity_update.hpp"

struct PhysicsManagementSettings {
    uint16_t tickRate = 320;
};

void entity_movement_update(GameData &gameData, DebugConfiguration &debugConfiguration, Position &position, PositionMutex &positionMutex, SpeedVector &speedVector, SpeedVectorMutex &speedVectorMutex, HitBoxRadius &hitBoxRadius, const float frameTime) {
    std::shared_lock<SpeedVectorMutex> speedVectorLock(speedVectorMutex);
    SpeedVector change = speedVector * frameTime;
    speedVectorLock.unlock();
    try_move_entity_with_deltaSpeed_change_on_collision(position, positionMutex, speedVector, speedVectorMutex, hitBoxRadius, gameData.map, change, debugConfiguration);
}

void entities_movement_update(GameData &gameData, DebugConfiguration &debugConfiguration, const float frameTime) {
    auto entityView = gameData.registry.view<Position, PositionMutex, SpeedVector, SpeedVectorMutex, Acceleration, HitBoxRadius>();
    std::shared_lock<MapMutex> mapLock(gameData.mapMutex);
    for (const entt::entity &entity : entityView) {
        const auto &[position, positionMutex, speedVector, speedVectorMutex, hitBoxRadius] = gameData.registry.get<Position, PositionMutex, SpeedVector, SpeedVectorMutex, HitBoxRadius>(entity);
        entity_movement_update(gameData, debugConfiguration, position, positionMutex, speedVector, speedVectorMutex, hitBoxRadius, frameTime);
    }
}

void players_cam_update(GameData &gameData, [[maybe_unused]] DebugConfiguration &debugConfiguration) {
    auto playerView = gameData.registry.view<PlayerMarker, Position, PositionMutex>();
    for (const entt::entity &player : playerView) {
        const auto &[position, positionMutex] = gameData.registry.get<Position, PositionMutex>(player);
        std::shared_lock<PositionMutex> positionLock(positionMutex);
        gameData.cam.target = position;
    }
}

void pickup_update(BodyRotation &bodyRotation, BodyRotationMutex &bodyRotationMutex, const float frameTime) {
    std::unique_lock bodyRotationLock(bodyRotationMutex);
    bodyRotation += 100.0f * frameTime;
}

void pickups_update(GameData &gameData, [[maybe_unused]] DebugConfiguration &debugConfiguration, const float frameTimeHundreths) {
    auto weaponView = gameData.registry.view<PickUpMarker, BodyRotation, BodyRotationMutex>();
    for (const entt::entity &weapon : weaponView) {
        const auto &[bodyRotation, bodyRotationMutex] = gameData.registry.get<BodyRotation, BodyRotationMutex>(weapon);
        pickup_update(bodyRotation, bodyRotationMutex, frameTimeHundreths);
    }
}

void update(GameData *gameData, DebugConfiguration *debugConfiguration, PhysicsManagementSettings *settings) {
    std::shared_lock<std::shared_mutex> runningLock(gameData->runningMutex);
    auto previous = std::chrono::steady_clock::now();

    double tickClock = 0;
    while (gameData->running) {
        auto now = std::chrono::steady_clock::now();

        double frameTime = std::chrono::duration<double>(now - previous).count();

        previous = now;

        runningLock.unlock();

        tickClock += frameTime;
        double tickTime = 1.0 / settings->tickRate;
        std::this_thread::sleep_for(std::chrono::duration<double>(tickTime - tickClock / 1.1));

        while(tickClock >= tickTime) {
            pickups_update(*gameData, *debugConfiguration, tickTime);
            game::loop::update::entities_friction(*gameData, *debugConfiguration, tickTime);
            entities_movement_update(*gameData, *debugConfiguration, tickTime);
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
        PhysicsManagement(GameData &gameData, DebugConfiguration &debugConfiguration, PhysicsManagementSettings settings) : gameData(gameData), debugConfiguration(debugConfiguration), settings(settings)  {
        }

        void start() {
            std::jthread thread(update, &gameData, &debugConfiguration, &this->settings);
            thread.detach();
        }
};

