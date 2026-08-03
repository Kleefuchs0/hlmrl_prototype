#include "DeltaSpeed.hpp"
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

                    if (x >= MAP_WIDTH || y >= MAP_HEIGHT) {
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

        template <size_t MAP_WIDTH, size_t MAP_HEIGHT>
        void try_move_player(Player &player, Map<MAP_WIDTH, MAP_HEIGHT> &map, DeltaSpeed change, DebugConfiguration &debugConfiguration) {
            Position oldPlayerPos = player.pos;
            player.pos.x += change.x;
            {
                std::vector<TileType> collisionTiles = get_player_map_collision(player, map, debugConfiguration);
                if(std::count(collisionTiles.begin(), collisionTiles.end(), TileType(tile_type::WALL))) {
                    player.pos.x = oldPlayerPos.x;
                    player.deltaSpeed.x *= -0.05;
                }
            }
            player.pos.y += change.y;
            {
                std::vector<TileType> collisionTiles = get_player_map_collision(player, map, debugConfiguration);
                if(std::count(collisionTiles.begin(), collisionTiles.end(), TileType(tile_type::WALL))) {
                    player.pos.y = oldPlayerPos.y;
                    player.deltaSpeed.y *= -0.05;
                }
            }
        }

        float get_player_angle_to_mouse(GameData &gameData) {
            Position mousePositionRelative = GetScreenToWorld2D(GetMousePosition() / (static_cast<float>(GetScreenWidth()) / gameData.worldWidth), gameData.cam) - gameData.player.pos;
            return std::atan2(mousePositionRelative.y, mousePositionRelative.x) * (180 / M_PI);
        }

        void game_input_update_player(GameData &gameData, DebugConfiguration &debugConfiguration) {
            Player &player = gameData.player;
            player.rotation = get_player_angle_to_mouse(gameData);

            bool invalidInput = true;
            EVector2 calculatedVector = {0, 0};
            if(IsKeyDown(KEY_W)) {
                calculatedVector.y -= 1;
                invalidInput = false;
            } else if(IsKeyDown(KEY_S)) {
                calculatedVector.y += 1;
                invalidInput = false;
            }
            if(IsKeyDown(KEY_D)) {
                calculatedVector.x += 1;
                invalidInput = false;
            } else if(IsKeyDown(KEY_A)) {
                calculatedVector.x -= 1;
                invalidInput = false;
            }


            float angle = std::atan2(calculatedVector.y, calculatedVector.x);
            DeltaSpeed playerDeltaSpeedChangeVector;
            if (invalidInput)
                playerDeltaSpeedChangeVector = {0, 0};
            else
                playerDeltaSpeedChangeVector = {cos(angle) * player.movementSpeed.value(), sin(angle) * player.movementSpeed.value()};

            player.deltaSpeed += playerDeltaSpeedChangeVector;
            try_move_player(player, gameData.map, player.deltaSpeed, debugConfiguration);

            gameData.cam.target = player.pos;
        }

        void game_input_update(GameData &gameData, DebugConfiguration &debugConfiguration) {
            game_input_update_player(gameData, debugConfiguration);
        }

        void misc_update(DebugConfiguration &debugConfiguration) {
            if(IsKeyPressed(KEY_F11))
                ToggleFullscreen();
            if (IsKeyPressed(KEY_F3))
                debugConfiguration.drawFPS = !debugConfiguration.drawFPS;

        }

        void entry(GameData &gameData, DebugConfiguration &debugConfiguration) {
            while (!WindowShouldClose()) {
                game::loop::misc_update(debugConfiguration);
                tick::tick_update(gameData, debugConfiguration);
                draw::draw(gameData, debugConfiguration);
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

void initialize_player(Player &player) {
    player.size.x = TILE_SIZE;
    player.size.y = TILE_SIZE;
    player.pos.x = TILE_SIZE * 2;
    player.pos.y = TILE_SIZE * 2;
    player.rotation = 70;
    player.movementSpeed = 0.1;
    player.circularHitBoxRadius = static_cast<float>(TILE_SIZE) / 2.5;
    player.deltaSpeed = {0, 0};
}

void initialize_map(GameData &gameData) {
    gameData.map.set_tile_type(0, 0, TileType(tile_type::WALL));
    gameData.map.set_tile_type(1, 0, TileType(tile_type::WALL));
    gameData.map.set_tile_type(2, 0, TileType(tile_type::WALL));
    gameData.map.set_tile_type(0, 1, TileType(tile_type::WALL));
    gameData.map.set_tile_type(0, 2, TileType(tile_type::WALL));
}

int main() {
    DebugConfiguration debugConfiguration;
    GameData gameData(generate_default_cam(640, 360), 640, 360);
    gameData.tickRate = 128;
    gameData.tickedFunctions["game_input"] = TickedFunction(1, &game::loop::game_input_update);
    initialize_player(gameData.player);
    initialize_map(gameData);
    InitWindow(gameData.worldWidth, gameData.worldHeight, "hlmrl");
    gameData.renderTexture = LoadRenderTexture(gameData.worldWidth, gameData.worldHeight);
    SetWindowSize(1280, 720);
    if (debugConfiguration.logLevel >= LogLevel::DEBUG)
        fmt::println("Entering game-loop");
    game::loop::entry(gameData, debugConfiguration);
    if (debugConfiguration.logLevel >= LogLevel::DEBUG)
        fmt::println("Closing and unloading game");
    CloseWindow();
    UnloadRenderTexture(gameData.renderTexture);
    if (debugConfiguration.logLevel >= LogLevel::DEBUG)
        fmt::println("Unlaoded everything");
    return 0;
}
