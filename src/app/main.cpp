#include "Acceleration.hpp"
#include "BodyRotation.hpp"
#include "DebugConfiguration.hpp"
#include "DeltaSpeed.hpp"
#include "EVector2.hpp"
#include "GameData.hpp"
#include "HitBoxRadius.hpp"
#include "LogLevel.hpp"
#include "PlayerMarker.hpp"
#include "Position.hpp"
#include "BodySize.hpp"
#include "constants.hpp"
#include "entt/entity/fwd.hpp"
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
        void try_move_entity(Position &position, DeltaSpeed &deltaSpeed, HitBoxRadius &hitBoxRadius, Map<MAP_WIDTH, MAP_HEIGHT> &map, DeltaSpeed change, DebugConfiguration &debugConfiguration) {
            Position oldPlayerPos = position;
            position.x += change.x;
            {
                std::array<TileType, 4> collisionTiles = get_map_collision_tiles(position, hitBoxRadius, map, debugConfiguration);
                if(std::count(collisionTiles.begin(), collisionTiles.end(), TileType(tile_type::WALL))) {
                    position.x = oldPlayerPos.x;
                    deltaSpeed.x *= -0.05;
                }
            }
            position.y += change.y;
            {
                std::array<TileType, 4> collisionTiles = get_map_collision_tiles(position, hitBoxRadius, map, debugConfiguration);
                if(std::count(collisionTiles.begin(), collisionTiles.end(), TileType(tile_type::WALL))) {
                    position.y = oldPlayerPos.y;
                    deltaSpeed.y *= -0.05;
                }
            }
        }

        float get_player_angle_to_mouse(const GameData &gameData, const Position &playerPosition) {
            Position mousePositionRelative = GetScreenToWorld2D(GetMousePosition() / (static_cast<float>(GetScreenWidth()) / gameData.worldWidth), gameData.cam) - playerPosition;
            return std::atan2(mousePositionRelative.y, mousePositionRelative.x) * (180 / M_PI);
        }

        void game_input_update_deltaspeed(DeltaSpeed &deltaSpeed, const Acceleration &acceleration) {

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
                playerDeltaSpeedChangeVector = {cos(angle) * acceleration.value(), sin(angle) * acceleration.value()};

            deltaSpeed += playerDeltaSpeedChangeVector;
        }

        void player_input_update(const entt::entity &player, GameData &gameData, DebugConfiguration &debugConfiguration) {
            BodyRotation &bodyRotation = gameData.registry.get<BodyRotation>(player);
            Position &position = gameData.registry.get<Position>(player);
            bodyRotation = get_player_angle_to_mouse(gameData, position);

            DeltaSpeed &deltaSpeed = gameData.registry.get<DeltaSpeed>(player);
            Acceleration &acceleration = gameData.registry.get<Acceleration>(player);
            game_input_update_deltaspeed(deltaSpeed, acceleration);
        }
        void players_input_update(GameData &gameData, DebugConfiguration &debugConfiguration) {
            auto playerView = gameData.registry.view<PlayerMarker, DeltaSpeed, BodyRotation, Acceleration>();
            for (const entt::entity &player : playerView) {
                player_input_update(player, gameData, debugConfiguration);
            }
        }

        void player_update(const entt::entity &player, GameData &gameData, DebugConfiguration &debugConfiguration) {
            DeltaSpeed &deltaSpeed = gameData.registry.get<DeltaSpeed>(player);
            Position &position = gameData.registry.get<Position>(player);
            HitBoxRadius &hitBoxRadius = gameData.registry.get<HitBoxRadius>(player);
            try_move_entity(position, deltaSpeed, hitBoxRadius, gameData.map, deltaSpeed, debugConfiguration);

            gameData.cam.target = position;
        }

        void players_update(GameData &gameData, DebugConfiguration &debugConfiguration) {
            auto playerView = gameData.registry.view<PlayerMarker, Position, DeltaSpeed, Acceleration, HitBoxRadius, BodyRotation>();
            for (const entt::entity &player : playerView) {
                player_update(player, gameData, debugConfiguration);
            }
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
    gameData.registry.emplace<PlayerMarker>(playerEntity);
    gameData.registry.emplace<Position>(playerEntity, TILE_SIZE * 2, TILE_SIZE * 2);
    gameData.registry.emplace<BodySize>(playerEntity, TILE_SIZE, TILE_SIZE);
    gameData.registry.emplace<BodyRotation>(playerEntity, 70);
    gameData.registry.emplace<Acceleration>(playerEntity, 0.1);
    gameData.registry.emplace<HitBoxRadius>(playerEntity, TILE_SIZE / 2.5);
    gameData.registry.emplace<DeltaSpeed>(playerEntity, 0, 0);
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
    gameData.tickedFunctions["players_input_update"] = TickedFunction(1, &game::loop::players_input_update);
    gameData.tickedFunctions["players_update"] = TickedFunction(1, &game::loop::players_update);
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
