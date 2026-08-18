#include "game/entity_movement_update.hpp"
#include "lib/Acceleration.hpp"
#include "lib/entity_try_move.hpp"

using namespace game;

void physics::movement::singular::update_entity_movement(GameData &gameData, DebugConfiguration &debugConfiguration, Position &position, SpeedVector &speedVector, HitBoxRadius &hitBoxRadius, const float frameTime) {
    SpeedVector change = speedVector * frameTime;
    try_move_entity_with_deltaSpeed_change_on_collision(position, speedVector, hitBoxRadius, gameData.map, change, debugConfiguration, frameTime);
}

void physics::movement::update_entities_movement(GameData &gameData, DebugConfiguration &debugConfiguration, const float frameTime) {
    std::shared_lock registryLock(gameData.registryMutex);
    auto entityView = gameData.registry.view<Position, SpeedVector,  Acceleration, HitBoxRadius>();
    std::shared_lock mapLock(gameData.mapMutex);
    for (const entt::entity &entity : entityView) {
        const auto &[position, speedVector, hitBoxRadius] = entityView.get<Position, SpeedVector, HitBoxRadius>(entity);
        singular::update_entity_movement(gameData, debugConfiguration, position, speedVector, hitBoxRadius, frameTime);
    }
}
