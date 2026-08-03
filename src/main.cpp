#include "EVector2.hpp"
#include "GameData.hpp"
#include "LogLevel.hpp"
#include "Player.hpp"
#include "Position.hpp"
#include "constants.hpp"
#include "fmt/core.h"
#include "game_tick.hpp"
#include "TickedFunction.hpp"
#include "game_draw.hpp"
#include "tile_type.hpp"
#include <cmath>
#include <cstddef>
#include <raylib.h>
#include <raymath.h>

namespace game {

    namespace loop {


        template <size_t MAP_WIDTH, size_t MAP_HEIGHT>
            std::vector<TileType> get_player_map_collision(Player &player, Map<MAP_WIDTH, MAP_HEIGHT> &map, DebugConfiguration &debugCfg) {
                std::pair<std::pair<size_t, size_t>, std::pair<size_t, size_t>> playerMapCoordinatesRange;

                playerMapCoordinatesRange.first = {(player.pos.x - player.circularHitBoxRadius) / TILE_SIZE, (player.pos.y - player.circularHitBoxRadius) / TILE_SIZE};
                playerMapCoordinatesRange.second = {std::ceil((player.pos.x + player.circularHitBoxRadius) / TILE_SIZE), std::ceil((player.pos.y + player.circularHitBoxRadius) / TILE_SIZE)};

                std::vector<TileType> collisionTileTypes;

                for (size_t y = playerMapCoordinatesRange.first.second; y < playerMapCoordinatesRange.second.second; y++) {
                    for (size_t x = playerMapCoordinatesRange.first.first; x < playerMapCoordinatesRange.second.first; x++) {

                        if (x > MAP_WIDTH || y > MAP_HEIGHT) {
                            if (debugCfg.logLevel >= LogLevel::WARNING)
                                fmt::println("Player has escaped the confines of the map at: {}, {}", x, y);
                            continue;
                        }

                        Rectangle tile = {
                            static_cast<float>(x * TILE_SIZE),
                            static_cast<float>(y * TILE_SIZE),
                            TILE_SIZE,
                            TILE_SIZE
                        };

                        if (CheckCollisionCircleRec(player.pos, player.circularHitBoxRadius, tile)==false)        // Skip if not colliding
                            continue;

                        collisionTileTypes.push_back(map.get_tile_type(x, y));
                    }
                }

                return collisionTileTypes;
            }

        void game_input_update_player(GameData &gameData) {
            Player &player = gameData.player;
            Position mousePositionRelative = GetScreenToWorld2D(GetMousePosition() / (static_cast<float>(GetScreenWidth()) / gameData.worldWidth), gameData.cam) - player.pos;
            player.rotation = std::atan2(mousePositionRelative.y, mousePositionRelative.x) * (180 / M_PI);
            EVector2 calculatedVector = {0, 0};
            bool noInput = true;
            Vector2 tempposition = player.pos;
            
            if(IsKeyDown(KEY_W)) {
                calculatedVector.y -= 1;
                noInput = false;
            }
            if(IsKeyDown(KEY_S)) {
                calculatedVector.y += 1;
                noInput = false;
            }
            if(IsKeyDown(KEY_D)) {
                calculatedVector.x += 1;
                noInput = false;
            }
            if(IsKeyDown(KEY_A)) {
                calculatedVector.x -= 1;
                noInput = false;
            }

            if (noInput)
                return;


            float angle = std::atan2(calculatedVector.y, calculatedVector.x);

            player.pos.x += cos(angle) * player.speed.value();
            {
                std::vector<TileType> collisionTiles = get_player_map_collision(player, gameData.map, gameData.debugConfiguration);
                if(std::count(collisionTiles.begin(), collisionTiles.end(), TileType(tile_type::WALL))) {
                    player.pos.x = tempposition.x;
                }
            }
            player.pos.y += sin(angle) * player.speed.value();
            {
                std::vector<TileType> collisionTiles = get_player_map_collision(player, gameData.map, gameData.debugConfiguration);
                if(std::count(collisionTiles.begin(), collisionTiles.end(), TileType(tile_type::WALL))) {
                    player.pos.y = tempposition.y;
                }
            }
            gameData.cam.target = player.pos;
        }

        void game_input_update(GameData &gameData) {
            game_input_update_player(gameData);
        }

        void misc_update() {
            if(IsKeyPressed(KEY_F11)) {
                ToggleFullscreen();
            }
        }

        void entry(GameData &gameData) {
            while (!WindowShouldClose()) {
                game::loop::misc_update();
                tick::tick_update(gameData);
                draw::draw(gameData);
            }
        }
    }
}

Camera2D generate_default_cam(int worldWidth, int worldHeight) {
    Camera2D cam;
    cam.offset = {static_cast<float>(worldWidth) / 2, static_cast<float>(worldHeight) / 2};
    cam.rotation = 0;
    cam.target = {0, 0};
    cam.zoom = 1;
    return cam;
}

void initialize_player(GameData &gameData) {
    gameData.player.size.x = TILE_SIZE;
    gameData.player.size.y = TILE_SIZE;
    gameData.player.pos.x = TILE_SIZE * 2;
    gameData.player.pos.y = TILE_SIZE * 2;
    gameData.player.rotation = 70;
    gameData.player.speed = 2.5;
    gameData.player.circularHitBoxRadius = static_cast<float>(TILE_SIZE) / 2.5;
}

void initialize_map(GameData &gameData) {
    gameData.map.set_tile_type(0, 0, TileType(tile_type::WALL));
    gameData.map.set_tile_type(1, 0, TileType(tile_type::WALL));
    gameData.map.set_tile_type(2, 0, TileType(tile_type::WALL));
    gameData.map.set_tile_type(0, 1, TileType(tile_type::WALL));
    gameData.map.set_tile_type(0, 2, TileType(tile_type::WALL));
}

int main() {
    GameData gameData(generate_default_cam(640, 360), 640, 360);
    gameData.tickRate = 128;
    gameData.tickedFunctions["game_input"] = TickedFunction(1, &game::loop::game_input_update);
    initialize_player(gameData);
    initialize_map(gameData);
    InitWindow(gameData.worldWidth, gameData.worldHeight, "hlmrl");
    gameData.renderTexture = LoadRenderTexture(gameData.worldWidth, gameData.worldHeight);
    SetTargetFPS(180);
    SetWindowSize(1280, 720);
    game::loop::entry(gameData);
    CloseWindow();
    UnloadRenderTexture(gameData.renderTexture);
    return 0;
}
