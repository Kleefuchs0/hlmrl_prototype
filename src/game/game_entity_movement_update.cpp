#include "game_entity_movement_update.hpp"
#include "Acceleration.hpp"
#include "entity_try_move.hpp"

using namespace game::loop;

void update::entity_movement_update(GameData &gameData, DebugConfiguration &debugConfiguration, Position &position, PositionMutex &positionMutex, SpeedVector &speedVector, SpeedVectorMutex &speedVectorMutex, HitBoxRadius &hitBoxRadius, const float frameTime) {
    std::shared_lock<SpeedVectorMutex> speedVectorLock(speedVectorMutex);
    SpeedVector change = speedVector * frameTime;
    speedVectorLock.unlock();
    try_move_entity_with_deltaSpeed_change_on_collision(position, positionMutex, speedVector, speedVectorMutex, hitBoxRadius, gameData.map, change, debugConfiguration, frameTime);
}

void update::entities_movement_update(GameData &gameData, DebugConfiguration &debugConfiguration, const float frameTime) {
    std::shared_lock registryLock(gameData.registryMutex);
    auto entityView = gameData.registry.view<Position, PositionMutex, SpeedVector, SpeedVectorMutex, Acceleration, HitBoxRadius>();
    std::shared_lock<MapMutex> mapLock(gameData.mapMutex);
    for (const entt::entity &entity : entityView) {
        const auto &[position, positionMutex, speedVector, speedVectorMutex, hitBoxRadius] = gameData.registry.get<Position, PositionMutex, SpeedVector, SpeedVectorMutex, HitBoxRadius>(entity);
        entity_movement_update(gameData, debugConfiguration, position, positionMutex, speedVector, speedVectorMutex, hitBoxRadius, frameTime);
    }
}
