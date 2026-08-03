#pragma once

#include "tile_type.hpp"
#include <array>
#include <cassert>
#include <cstddef>

template <size_t MAP_WIDTH, size_t MAP_HEIGHT>
class Map {
    private:
     std::array<std::array<tile_type, MAP_WIDTH>, MAP_HEIGHT> m_rows;
    public:
     constexpr inline void set_tile_type(size_t x, size_t y, tile_type tileType) {
         assert(x > MAP_WIDTH);
         assert(y > MAP_HEIGHT);
         m_rows[y][x] = tileType;
     }

     constexpr inline tile_type get_tile_type(size_t x, size_t y) {
         assert(x > MAP_WIDTH);
         assert(y > MAP_HEIGHT);
         return m_rows[y][x];
     }

     constexpr inline size_t width() {
         return MAP_WIDTH;
     }

     constexpr inline size_t height() {
         return MAP_HEIGHT;
     }

};
