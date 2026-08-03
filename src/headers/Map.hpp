#pragma once

#include "constants.hpp"
#include "tile_type.hpp"
#include <array>
#include <cassert>
#include <cstddef>
#include <raylib.h>
template <size_t MAP_WIDTH, size_t MAP_HEIGHT>
class Map {
    private:
     std::array<std::array<TileType, MAP_WIDTH>, MAP_HEIGHT> m_rows;
    public:
     constexpr inline void set_tile_type(size_t x, size_t y, TileType tileType) {
         assert(x < MAP_WIDTH);
         assert(y < MAP_HEIGHT);
         m_rows[y][x] = tileType;
     }

     constexpr inline TileType get_tile_type(size_t x, size_t y) {
         assert(x < MAP_WIDTH);
         assert(y < MAP_HEIGHT);
         return m_rows[y][x];
     }

     constexpr inline size_t width() {
         return MAP_WIDTH;
     }

     constexpr inline size_t height() {
         return MAP_HEIGHT;
     }
     bool check_for_collision(Rectangle object) {

    for (size_t y = 0; y < MAP_HEIGHT; y++) {
        for (size_t x = 0; x < MAP_WIDTH; x++) {

            if (m_rows[y][x].value() == tile_type::WALL) {

                Rectangle tile = {
                    static_cast<float>(x * TILE_SIZE),
                    static_cast<float>(y * TILE_SIZE),
                    TILE_SIZE,
                    TILE_SIZE
                };

                if (CheckCollisionRecs(object, tile)) {
                    return true;
                }
            }
        }
    }

    return false;
}
};