#include "DebugConfiguration.hpp"
#include "LogLevel.hpp"
#include "Map.hpp"
#include "Position.hpp"
#include "catch2/catch_test_macros.hpp"
#include "entt/entt.hpp"
#include "tile_type.hpp"
#include <catch2/catch_all.hpp>
#include <entity_map_interaction.hpp>
#include <entity_try_move.hpp>

TEST_CASE("Testing functions for entity game interactions", "[entity_game_interaction]") {
    SECTION("Testing entity collision detection with tiles") {
        Map<8, 8, 64.0f> map;
        for (size_t y = 0; y < map.height(); y++) {
            for (size_t x = 0; x < map.width(); x++) {
                TileType tileType = (y % 2 == 0) ? ((x % 2 == 0) ? TileType(tile_type::EMPTY) : TileType(tile_type::FLOOR)) : ((x % 2 == 1) ? TileType(tile_type::EMPTY) : TileType(tile_type::FLOOR));
                map.set_tile_type(x, y, tileType);
            }
        }
        Position pos = {64.0f, 64.0f};
        HitBoxRadius radius = 32.0f;
        DebugConfiguration debugConfiguration;
        debugConfiguration.logLevel = LogLevel::DEBUG;
        std::array<TileType, 4> collisionTiles = get_map_collision_tiles(pos, radius, map, debugConfiguration);
        REQUIRE(collisionTiles[0] == TileType(tile_type::EMPTY));
        REQUIRE(collisionTiles[1] == TileType(tile_type::FLOOR));
        REQUIRE(collisionTiles[2] == TileType(tile_type::FLOOR));
        REQUIRE(collisionTiles[3] == TileType(tile_type::EMPTY));
    }

    SECTION("Testing entity collision detection, with tiles") {
        Map<8, 8, 64.0f> map;
        for (size_t y = 0; y < map.height(); y++) {
            for (size_t x = 0; x < map.width(); x++) {
                map.set_tile_type(x, y, tile_type::FLOOR);
            }
        }

        Position pos = {64.0f, 64.0f};
        HitBoxRadius radius = 32.0f;
        DebugConfiguration debugConfiguration;
        debugConfiguration.logLevel = LogLevel::DEBUG;

        try_move_entity(pos, radius, map, {0, 0}, debugConfiguration);
    }
}
