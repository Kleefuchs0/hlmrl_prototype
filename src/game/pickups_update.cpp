#include "game/pickups_update.hpp"
#include "lib/PickUpMarker.hpp"

using namespace game;

void physics::singular::update_pickup(BodyRotation &bodyRotation, const float frameTime) {
    bodyRotation += 100.0f * frameTime;
}

void physics::update_pickups(GameData &gameData, [[maybe_unused]] DebugConfiguration &debugConfiguration, const float frameTime) {
    auto weaponView = gameData.registry.view<PickUpMarker, BodyRotation>();
    for (const entt::entity &weapon : weaponView) {
        BodyRotation &bodyRotation = weaponView.get<BodyRotation>(weapon);
        singular::update_pickup(bodyRotation, frameTime);
    }
}

