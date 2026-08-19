#pragma once

#include "DebugConfiguration.hpp"
#include "entity_map_interaction.hpp"
#include "SpeedVector.hpp"
#include "HitBoxRadius.hpp"
#include "Map.hpp"
#include "Position.hpp"
#include <algorithm>

/*
 * A return value of 3 means both moves were successful                         (both bits)
 * A return value of 2 means y-Axis move was successful, and x-Axis was not.    (second bit)
 * A return value of 1 means x-Axis move was successful, and y-Axis was not.    (first bit)
 * A return value of 0 means none were sucessful
 *
 * In other words:
 * The first bit states wether x-Axis move was successful,
 * and the second bit states wether y-Axis move was successful
*/
enum entity_move_return_code {
    NONE_MOVED  =   0,
    X_MOVED     =   1,
    Y_MOVED     =   2,
    BOTH_MOVED  =   3
};

template <size_t MAP_WIDTH, size_t MAP_HEIGHT, float TILE_SIZE>
entity_move_return_code try_move_entity(Position &position, const HitBoxRadius &hitBoxRadius, const Map<MAP_WIDTH, MAP_HEIGHT, TILE_SIZE> &map, const SpeedVector change) {
    entity_move_return_code retval = entity_move_return_code::BOTH_MOVED;
    Position oldPlayerPos = position;
    position.x += change.x;
    {
        std::array<TileType, 4> collisionTiles = get_map_collision_tiles(position, hitBoxRadius, map);
        if(std::count(collisionTiles.begin(), collisionTiles.end(), TileType(tile_type::WALL))) {
            position.x = oldPlayerPos.x;
            retval = static_cast<entity_move_return_code>(static_cast<int>(retval) - entity_move_return_code::X_MOVED);
        }
    }
    position.y += change.y;
    {
        std::array<TileType, 4> collisionTiles = get_map_collision_tiles(position, hitBoxRadius, map);
        if(std::count(collisionTiles.begin(), collisionTiles.end(), TileType(tile_type::WALL))) {
            position.y = oldPlayerPos.y;
            retval = static_cast<entity_move_return_code>(static_cast<int>(retval) - entity_move_return_code::Y_MOVED);
        }
    }

    return retval;
}

template <size_t MAP_WIDTH, size_t MAP_HEIGHT, float TILE_SIZE>
entity_move_return_code try_move_entity_with_deltaSpeed_change_on_collision(Position &position, SpeedVector &speedVector, const HitBoxRadius &hitBoxRadius, const Map<MAP_WIDTH, MAP_HEIGHT, TILE_SIZE> &map, const SpeedVector change) {
    entity_move_return_code retval = try_move_entity(position, hitBoxRadius, map, change);

    switch (retval) {
        case entity_move_return_code::X_MOVED:
            speedVector.y *= -0.1f;
            break;
        case entity_move_return_code::Y_MOVED:
            speedVector.x *= -0.1f;
            break;
        case entity_move_return_code::NONE_MOVED:
            speedVector *= -0.1f;
            break;
        case entity_move_return_code::BOTH_MOVED:
            return retval;
    }

    return retval;
}
