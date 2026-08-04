
#include "DebugConfiguration.hpp"
#include "DeltaSpeed.hpp"
#include "HitBoxRadius.hpp"
#include "Map.hpp"
#include "Position.hpp"
#include <algorithm>

template <size_t MAP_WIDTH, size_t MAP_HEIGHT>
void try_move_entity(Position &position, DeltaSpeed &deltaSpeed, HitBoxRadius &hitBoxRadius, Map<MAP_WIDTH, MAP_HEIGHT> &map, DeltaSpeed change, DebugConfiguration &debugConfiguration) {
    Position oldPlayerPos = position;
    position.x += change.x;
    {
        std::array<TileType, 4> collisionTiles = get_map_collision_tiles(position, hitBoxRadius, map, debugConfiguration);
        if(std::count(collisionTiles.begin(), collisionTiles.end(), TileType(tile_type::WALL))) {
            position.x = oldPlayerPos.x;
            deltaSpeed.x *= -0.05;
        }
    }
    position.y += change.y;
    {
        std::array<TileType, 4> collisionTiles = get_map_collision_tiles(position, hitBoxRadius, map, debugConfiguration);
        if(std::count(collisionTiles.begin(), collisionTiles.end(), TileType(tile_type::WALL))) {
            position.y = oldPlayerPos.y;
            deltaSpeed.y *= -0.05;
        }
    }
}

