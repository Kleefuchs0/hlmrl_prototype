#pragma once

#include "tile_type.hpp"
#include <array>
#include <cassert>
#include <cstddef>

template <size_t MAP_WIDTH, size_t MAP_HEIGHT>
class Map {
    private:
     std::array<tile_type, MAP_WIDTH * MAP_HEIGHT> m_tiles;
    public:
     constexpr inline void set_tile_type(size_t x, size_t y, tile_type tileType) {
         assert(x * y > m_tiles.size());
         m_tiles[x * y] = tileType;
     }

     constexpr inline tile_type get_tile_type(size_t x, size_t y) {
         assert(x * y > m_tiles.size());
         return m_tiles[x * y];
     }
};
