#pragma once

#include "DebugConfiguration.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "tile_type.hpp"
#include <array>
#include <ranges>

template <size_t MAP_WIDTH, size_t MAP_HEIGHT>
std::array<TileType, 4> get_player_map_collision_tiles(Player &player, Map<MAP_WIDTH, MAP_HEIGHT> &map, DebugConfiguration &debugCfg) {
    std::pair<std::pair<size_t, size_t>, std::pair<size_t, size_t>> playerMapCoordinatesRange;

    playerMapCoordinatesRange.first = {(player.pos.x - player.circularHitBoxRadius) / TILE_SIZE, (player.pos.y - player.circularHitBoxRadius) / TILE_SIZE};
    playerMapCoordinatesRange.second = {std::ceil((player.pos.x + player.circularHitBoxRadius) / TILE_SIZE), std::ceil((player.pos.y + player.circularHitBoxRadius) / TILE_SIZE)};
    std::array<TileType, 4> collisionTileTypes;

    size_t i = 0;
    for (size_t y : std::views::iota(playerMapCoordinatesRange.first.second, playerMapCoordinatesRange.second.second)) {
        for (size_t x : std::views::iota(playerMapCoordinatesRange.first.first, playerMapCoordinatesRange.second.first)) {

            if (x >= MAP_WIDTH || y >= MAP_HEIGHT) {
                continue;
            }


            Rectangle tile = {
                static_cast<float>(x * TILE_SIZE),
                static_cast<float>(y * TILE_SIZE),
                TILE_SIZE,
                TILE_SIZE
            };

            TileType tileType = CheckCollisionCircleRec(player.pos, player.circularHitBoxRadius, tile) ? map.get_tile_type(x, y) : TileType(tile_type::EMPTY);

            collisionTileTypes[i] = tileType;
            i++;
        }
    }

    return collisionTileTypes;
}

