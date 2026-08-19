#pragma once

#include "HitBoxRadius.hpp"
#include "Map.hpp"
#include "Position.hpp"
#include "tile_type.hpp"
#include <array>
#include <fmt/base.h>
#include <ranges>

template <size_t MAP_WIDTH, size_t MAP_HEIGHT, float TILE_SIZE>
std::array<TileType, 4> get_map_collision_tiles(const Position &position, const HitBoxRadius &hitBoxRadius, const Map<MAP_WIDTH, MAP_HEIGHT, TILE_SIZE> &map) {
    std::pair<std::pair<size_t, size_t>, std::pair<size_t, size_t>> playerMapCoordinatesRange;

    playerMapCoordinatesRange.first = {(position.x - hitBoxRadius.value()) / TILE_SIZE, (position.y - hitBoxRadius.value()) / TILE_SIZE};
    playerMapCoordinatesRange.second = {std::ceil((position.x + hitBoxRadius.value()) / TILE_SIZE), std::ceil((position.y + hitBoxRadius.value()) / TILE_SIZE)};
    std::array<TileType, 4> collisionTileTypes;

    // if (playerMapCoordinatesRange.first.first >= MAP_WIDTH)     // Correct too big values
    //     playerMapCoordinatesRange.first.first = 0;
    // if (playerMapCoordinatesRange.first.second >= MAP_HEIGHT)     // Correct too big values
    //     playerMapCoordinatesRange.first.second = 0;
    // if (playerMapCoordinatesRange.second.first >= MAP_WIDTH)     // Correct too big values
    //     playerMapCoordinatesRange.second.first = 1;
    // if (playerMapCoordinatesRange.second.second >= MAP_HEIGHT)     // Correct too big values
    //     playerMapCoordinatesRange.second.second = 1;

    size_t i = 0;
    for (size_t y : std::views::iota(playerMapCoordinatesRange.first.second, playerMapCoordinatesRange.second.second)) {
        for (size_t x : std::views::iota(playerMapCoordinatesRange.first.first, playerMapCoordinatesRange.second.first)) {

            Rectangle tile = {
                static_cast<float>(x * TILE_SIZE),
                static_cast<float>(y * TILE_SIZE),
                TILE_SIZE,
                TILE_SIZE
            };

            TileType tileType = CheckCollisionCircleRec(position, hitBoxRadius.value(), tile) || (x >= MAP_WIDTH || y >= MAP_HEIGHT) ? map.get_tile_type(x, y) : TileType(tile_type::EMPTY);

            collisionTileTypes[i] = tileType;
            i++;
        }
    }

    return collisionTileTypes;
}

