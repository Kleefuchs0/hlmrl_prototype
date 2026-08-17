#include <game_entity_update.hpp>

using namespace game::loop;

void update::entity_floor_friction(GameData &gameData, const DebugConfiguration &debugConfiguration, Position &position, SpeedVector &speedVector, HitBoxRadius &hitBoxRadius, SpecificFloorFrictionSlowdown &specificFloorFrictionSlowdown, const float frameTime) {
    std::array<TileType, 4> tiles = get_map_collision_tiles(position, hitBoxRadius, gameData.map, debugConfiguration);
    for (TileType tile : tiles) {
        if (tile.value() >= TILE_TYPE_SECTION_START_FLOORS && tile.value() < TILE_TYPE_SECTION_START_WALLS) {
            speedVector /= 1 + specificFloorFrictionSlowdown.value() * frameTime;
        }
    }
}

void update::entities_friction(GameData &gameData, DebugConfiguration &debugConfiguration, const float frameTime) {
    std::shared_lock registryLock(gameData.registryMutex);
    auto entityView = gameData.registry.view<Position, SpeedVector, HitBoxRadius, SpecificFloorFrictionSlowdown>();
    std::shared_lock mapLock(gameData.mapMutex);
    for (const entt::entity &entity : entityView) {
        const auto &[position, speedVector, hitBoxRadius, specificFloorFritionSlowdown] = gameData.registry.get<Position, SpeedVector, HitBoxRadius, SpecificFloorFrictionSlowdown>(entity);
        entity_floor_friction(gameData, debugConfiguration, position, speedVector, hitBoxRadius, specificFloorFritionSlowdown, frameTime);
    }
}
