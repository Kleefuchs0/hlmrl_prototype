#include "game/pickups_update.hpp"
#include "lib/PickUpMarker.hpp"
#include <mutex>

using namespace game;

void physics::singular::update_pickup(BodyRotation &bodyRotation, BodyRotationMutex &bodyRotationMutex, const float frameTime) {
    std::unique_lock bodyRotationLock(bodyRotationMutex);
    bodyRotation += 100.0f * frameTime;
}

void physics::update_pickups(GameData &gameData, [[maybe_unused]] DebugConfiguration &debugConfiguration, const float frameTime) {
    std::shared_lock registryLock(gameData.registryMutex);
    auto weaponView = gameData.registry.view<PickUpMarker, BodyRotation, BodyRotationMutex>();
    for (const entt::entity &weapon : weaponView) {
        const auto &[bodyRotation, bodyRotationMutex] = gameData.registry.get<BodyRotation, BodyRotationMutex>(weapon);
        singular::update_pickup(bodyRotation, bodyRotationMutex, frameTime);
    }
}

