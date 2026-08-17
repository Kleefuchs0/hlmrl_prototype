#include "Acceleration.hpp"
#include "BodyRotation.hpp"
#include "BodyRotationMutex.hpp"
#include "DebugConfiguration.hpp"
#include "PhysicsManagement.hpp"
#include "PickUpMarker.hpp"
#include "PositionMutex.hpp"
#include "SpecificFloorFrictionSlowdown.hpp"
#include "SpeedVector.hpp"
#include "EVector2.hpp"
#include "GameData.hpp"
#include "HitBoxRadius.hpp"
#include "LogLevel.hpp"
#include "PlayerMarker.hpp"
#include "Position.hpp"
#include "BodySize.hpp"
#include "SpeedVectorMutex.hpp"
#include "WeaponMarker.hpp"
#include "constants.hpp"
#include <game_entity_update.hpp>
#include "entt/entity/fwd.hpp"
#include "fmt/core.h"
#include "game_tick.hpp"
#include "TickedFunction.hpp"
#include "game_draw.hpp"
#include "tile_type.hpp"
#include <cmath>
#include <cstddef>
#include <mutex>
#include <raylib.h>
#include <raymath.h>
#include <shared_mutex>
#include "entity_try_move.hpp"

namespace game {

    namespace loop {


        float get_player_angle_to_mouse(const GameData &gameData, const Position &playerPosition, PositionMutex &positionMutex) {
            std::shared_lock<PositionMutex> positionLock(positionMutex);
            Position mousePositionRelative = GetScreenToWorld2D(GetMousePosition() / (static_cast<float>(GetScreenWidth()) / static_cast<float>(gameData.worldWidth)), gameData.cam) - playerPosition;
            positionLock.unlock();
            return static_cast<float>(std::atan2(static_cast<double>(mousePositionRelative.y), static_cast<double>(mousePositionRelative.x))) * static_cast<float>(180 / M_PI);
        }

        void player_input_update_speedvector(SpeedVector &speedVector, SpeedVectorMutex &speedVectorMutex, const Acceleration &acceleration) {


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
            SpeedVector playerDeltaSpeedChangeVector;
            if (invalidInput)
                playerDeltaSpeedChangeVector = {0, 0};
            else
                playerDeltaSpeedChangeVector = {cos(angle) * acceleration.value(), sin(angle) * acceleration.value()};

            std::unique_lock<SpeedVectorMutex> speedVectorLock(speedVectorMutex);
            speedVector += playerDeltaSpeedChangeVector;
        }

        void player_input_update(GameData &gameData, [[maybe_unused]] DebugConfiguration &debugConfiguration, Position &position, PositionMutex &positionMutex, SpeedVector &speedVector, SpeedVectorMutex &speedVectorMutex, Acceleration &acceleration, BodyRotation &bodyRotation, BodyRotationMutex &bodyRotationMutex) {
            std::unique_lock<BodyRotationMutex> bodyRotationLock(bodyRotationMutex);
            bodyRotation = get_player_angle_to_mouse(gameData, position, positionMutex);
            bodyRotationLock.unlock();

            player_input_update_speedvector(speedVector, speedVectorMutex, acceleration);
        }

        void players_input_update(GameData &gameData, DebugConfiguration &debugConfiguration) {
            auto playerView = gameData.registry.view<PlayerMarker, Position, PositionMutex, SpeedVector, SpeedVectorMutex, BodyRotation, BodyRotationMutex, Acceleration>();
            for (const entt::entity &player : playerView) {
                const auto &[bodyRotation, bodyRotationMutex, position, positionMutex, speedVector, speedVectorMutex, acceleration] = gameData.registry.get<BodyRotation, BodyRotationMutex, Position, PositionMutex, SpeedVector, SpeedVectorMutex, Acceleration>(player);
                player_input_update(gameData, debugConfiguration, position, positionMutex, speedVector, speedVectorMutex, acceleration, bodyRotation, bodyRotationMutex);
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
    gameData.registry.emplace<Position>(playerEntity, gameData.map.tile_size() * 2, gameData.map.tile_size() * 2);
    gameData.registry.emplace<PositionMutex>(playerEntity);
    gameData.registry.emplace<BodySize>(playerEntity, gameData.map.tile_size(), gameData.map.tile_size());
    gameData.registry.emplace<BodyRotation>(playerEntity, 70);
    gameData.registry.emplace<BodyRotationMutex>(playerEntity);
    gameData.registry.emplace<Acceleration>(playerEntity, 20);
    gameData.registry.emplace<HitBoxRadius>(playerEntity, gameData.map.tile_size() / 2.5);
    gameData.registry.emplace<SpeedVector>(playerEntity, 0, 0);
    gameData.registry.emplace<SpeedVectorMutex>(playerEntity);
    gameData.registry.emplace<SpecificFloorFrictionSlowdown>(playerEntity, 2);
}

void initialize_test_weapon(GameData &gameData) {
    auto weaponEntity = gameData.registry.create();
    gameData.registry.emplace<WeaponMarker>(weaponEntity);
    gameData.registry.emplace<PickUpMarker>(weaponEntity);
    gameData.registry.emplace<Position>(weaponEntity, gameData.map.tile_size() * 4, gameData.map.tile_size() * 4);
    gameData.registry.emplace<PositionMutex>(weaponEntity);
    gameData.registry.emplace<BodySize>(weaponEntity, gameData.map.tile_size() / 2, gameData.map.tile_size() / 2);
    gameData.registry.emplace<HitBoxRadius>(weaponEntity, gameData.map.tile_size() / 5);
    gameData.registry.emplace<BodyRotation>(weaponEntity, 0);
    gameData.registry.emplace<BodyRotationMutex>(weaponEntity);
}

void initialize_map(GameData &gameData) {

    for (size_t x = 0; x < DEFAULT_MAP_WIDTH; x++) {
        gameData.map.set_tile_type(x, 0, TileType(tile_type::WALL));
        gameData.map.set_tile_type(x, DEFAULT_MAP_HEIGHT - 1, TileType(tile_type::WALL));
    }

    for (size_t y = 1; y < DEFAULT_MAP_HEIGHT; y++) {
        gameData.map.set_tile_type(0, y, TileType(tile_type::WALL));
        gameData.map.set_tile_type(DEFAULT_MAP_HEIGHT - 1, y, TileType(tile_type::WALL));
    }

    for (size_t x = 1; x < DEFAULT_MAP_WIDTH - 1; x++) 
        for (size_t y = 1; y < DEFAULT_MAP_HEIGHT - 1; y++)
            gameData.map.set_tile_type(x, y, TileType(tile_type::FLOOR));
}

int main() {
    DebugConfiguration debugConfiguration;
    GameData gameData(generate_default_cam(640, 360), 640, 360);
    gameData.tickRate = 128;
    initialize_player(gameData);
    initialize_map(gameData);
    initialize_test_weapon(gameData);
    gameData.tickedFunctions["players_input_update"] = TickedFunction(1, &game::loop::players_input_update);
    PhysicsManagement physicsManagement(gameData, debugConfiguration, 560);
    InitWindow(gameData.worldWidth, gameData.worldHeight, "hlmrl");
    SetTargetFPS(640);
    gameData.renderTexture = LoadRenderTexture(gameData.worldWidth, gameData.worldHeight);
    SetWindowSize(1280, 720);
    if (debugConfiguration.logLevel >= LogLevel::DEBUG)
        fmt::println("Entering game-loop");
    physicsManagement.start();
    game::loop::entry(gameData, debugConfiguration);
    if (debugConfiguration.logLevel >= LogLevel::DEBUG)
        fmt::println("Closing and unloading game");
    UnloadRenderTexture(gameData.renderTexture);
    CloseWindow();
    std::unique_lock<std::shared_mutex> runningLock(gameData.runningMutex);
    gameData.running = false;
    runningLock.unlock();
    if (debugConfiguration.logLevel >= LogLevel::DEBUG)
        fmt::println("Unlaoded everything");
    return 0;
}
