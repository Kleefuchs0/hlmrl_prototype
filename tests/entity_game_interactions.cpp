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

    SECTION("Testing entity movement") {
        Map<8, 8, 64.0f> map;
        for (size_t y = 0; y < map.height(); y++) {
            for (size_t x = 0; x < map.width(); x++) {
                map.set_tile_type(x, y, tile_type::FLOOR);
            }
        }
        map.set_tile_type(0, 0, tile_type::WALL);
        map.set_tile_type(0, 1, tile_type::WALL);
        map.set_tile_type(0, 2, tile_type::WALL);
        map.set_tile_type(1, 0, tile_type::WALL);
        map.set_tile_type(2, 0, tile_type::WALL);

        Position pos = {64.0f * 2, 64.0f * 2};
        HitBoxRadius radius = 32.0f;
        DebugConfiguration debugConfiguration;
        debugConfiguration.logLevel = LogLevel::DEBUG;

        {
            entity_move_return_code retval = try_move_entity(pos, radius, map, {-32, -32}, debugConfiguration);
            REQUIRE(retval == entity_move_return_code::BOTH_MOVED);
            REQUIRE(pos.x == 96.0f);
            REQUIRE(pos.y == 96.0f);
        }

        {
            entity_move_return_code retval = try_move_entity(pos, radius, map, {-16, 0}, debugConfiguration);
            REQUIRE(retval == entity_move_return_code::Y_MOVED);
            REQUIRE(pos.x == 96.0f);
            REQUIRE(pos.y == 96.0f);
        }

        {
            entity_move_return_code retval = try_move_entity(pos, radius, map, {-16, -16}, debugConfiguration);
            REQUIRE(retval == entity_move_return_code::NONE_MOVED);
            REQUIRE(pos.x == 96.0f);
            REQUIRE(pos.y == 96.0f);
        }
    }
}
