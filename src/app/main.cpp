#include "BodyRotation.hpp"
#include "DebugConfiguration.hpp"
#include "DeltaSpeed.hpp"
#include "EVector2.hpp"
#include "GameData.hpp"
#include "HitBoxRadius.hpp"
#include "LogLevel.hpp"
#include "Player.hpp"
#include "Position.hpp"
#include "BodySize.hpp"
#include "constants.hpp"
#include "fmt/core.h"
#include "game_tick.hpp"
#include "TickedFunction.hpp"
#include "game_draw.hpp"
#include "tile_type.hpp"
#include <array>
#include <cmath>
#include <cstddef>
#include <raylib.h>
#include <raymath.h>
#include "player_map_interaction.hpp"

namespace game {

    namespace loop {


        template <size_t MAP_WIDTH, size_t MAP_HEIGHT>
        void try_move_player(Player &player, Map<MAP_WIDTH, MAP_HEIGHT> &map, DeltaSpeed change, DebugConfiguration &debugConfiguration) {
            Position oldPlayerPos = player.pos;
            player.pos.x += change.x;
            {
                std::array<TileType, 4> collisionTiles = get_player_map_collision_tiles(player, map, debugConfiguration);
                if(std::count(collisionTiles.begin(), collisionTiles.end(), TileType(tile_type::WALL))) {
                    player.pos.x = oldPlayerPos.x;
                    player.deltaSpeed.x *= -0.05;
                }
            }
            player.pos.y += change.y;
            {
                std::array<TileType, 4> collisionTiles = get_player_map_collision_tiles(player, map, debugConfiguration);
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

        void player_update(GameData &gameData, DebugConfiguration &debugConfiguration) {
            game_input_update_player(gameData, debugConfiguration);
        }

        void misc_update(DebugConfiguration &debugConfiguration) {
            if(IsKeyPressed(KEY_F11))
                ToggleFullscreen();
            if (IsKeyPressed(KEY_F3))
                debugConfiguration.drawFPS = !debugConfiguration.drawFPS;
            if (IsKeyPressed(KEY_F10)) {
                if (IsKeyDown(KEY_LEFT_SHIFT) && debugConfiguration.drawHitBoxes) {
                    debugConfiguration.currentHitBoxColor++;

                    if (debugConfiguration.currentHitBoxColor >= debugConfiguration.avaivableHitBoxColors.size()) 
                        debugConfiguration.currentHitBoxColor = 0;

                    debugConfiguration.hitBoxColor = debugConfiguration.avaivableHitBoxColors[debugConfiguration.currentHitBoxColor];
                } else {
                    debugConfiguration.drawHitBoxes = !debugConfiguration.drawHitBoxes;
                }
            }

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

void initialize_player(GameData &gameData) {
    auto playerEntity = gameData.registry.create();
    gameData.registry.emplace<Position>(playerEntity, TILE_SIZE * 2, TILE_SIZE * 2);
    gameData.registry.emplace<BodySize>(playerEntity, TILE_SIZE, TILE_SIZE);
    gameData.registry.emplace<BodyRotation>(playerEntity, 70);
    gameData.registry.emplace<Acceleration>(playerEntity, 0.1);
    gameData.registry.emplace<HitBoxRadius>(playerEntity, TILE_SIZE / 2.5);
    gameData.registry.emplace<DeltaSpeed>(playerEntity, 0, 0);
    gameData.registry.emplace<TickedFunction>(playerEntity, TickedFunction(1, &game::loop::player_update));
    Player &player = gameData.player;
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

    for (size_t x = 0; x <= 100; x++) {
        gameData.map.set_tile_type(x, 0, TileType(tile_type::WALL));
        gameData.map.set_tile_type(x, 100, TileType(tile_type::WALL));
    }

    for (size_t y = 1; y <= 100; y++) {
        gameData.map.set_tile_type(0, y, TileType(tile_type::WALL));
        gameData.map.set_tile_type(100, y, TileType(tile_type::WALL));
    }

    for (size_t x = 1; x < 100; x++) 
        for (size_t y = 1; y < 100; y++)
            gameData.map.set_tile_type(x, y, TileType(tile_type::FLOOR));
}

int main() {
    DebugConfiguration debugConfiguration;
    GameData gameData(generate_default_cam(640, 360), 640, 360);
    gameData.tickRate = 128;
    initialize_player(gameData);
    initialize_map(gameData);
    InitWindow(gameData.worldWidth, gameData.worldHeight, "hlmrl");
    gameData.renderTexture = LoadRenderTexture(gameData.worldWidth, gameData.worldHeight);
    SetWindowSize(1280, 720);
    if (debugConfiguration.logLevel >= LogLevel::DEBUG)
        fmt::println("Entering game-loop");
    game::loop::entry(gameData, debugConfiguration);
    if (debugConfiguration.logLevel >= LogLevel::DEBUG)
        fmt::println("Closing and unloading game");
    UnloadRenderTexture(gameData.renderTexture);
    CloseWindow();
    if (debugConfiguration.logLevel >= LogLevel::DEBUG)
        fmt::println("Unlaoded everything");
    return 0;
}
