#include "game/ConstantRenderData.hpp"
#include "game/InputData.hpp"
#include "game/RenderData.hpp"
#include "lib/Acceleration.hpp"
#include "lib/BodyRotation.hpp"
#include "lib/DebugConfiguration.hpp"
#include "game/PhysicsManagement.hpp"
#include "lib/PickUpMarker.hpp"
#include "lib/SpecificFloorFrictionSlowdown.hpp"
#include "lib/SpeedVector.hpp"
#include "lib/EVector2.hpp"
#include "lib/GameData.hpp"
#include "lib/HitBoxRadius.hpp"
#include "lib/LogLevel.hpp"
#include "lib/PlayerMarker.hpp"
#include "lib/Position.hpp"
#include "lib/BodySize.hpp"
#include "lib/WeaponMarker.hpp"
#include "lib/constants.hpp"
#include "fmt/core.h"
#include "lib/TickedFunction.hpp"
#include "game/rendering.hpp"
#include "lib/tile_type.hpp"
#include <cstddef>
#include <mutex>
#include <raylib.h>
#include <raymath.h>
#include <shared_mutex>

namespace game {

    namespace input {


        EVector2 player_input_get_relative_movement() {

            EVector2 calculatedVector = {0, 0};
            if(IsKeyDown(KEY_W)) {
                calculatedVector.y -= 1.0f;
            } else if(IsKeyDown(KEY_S)) {
                calculatedVector.y += 1.0f;
            }
            if(IsKeyDown(KEY_D)) {
                calculatedVector.x += 1.0f;
            } else if(IsKeyDown(KEY_A)) {
                calculatedVector.x -= 1.0f;
            }

            return calculatedVector;
        }

        InputData player_input_get([[maybe_unused]] DebugConfiguration &debugConfiguration) {
            InputData data;
            data.cursorPosition = GetMousePosition();
            data.relativeMovement = player_input_get_relative_movement();
            return data;
        }
    }

    void entry(DebugConfiguration &debugConfiguration, InputData &nextInputData, bool &nextInputDataRenewed, std::shared_mutex &nextInputDataMutex, ConstantRenderData &constantRenderData, RenderData &currentRenderData, RenderData &newestRenderData, bool &newestRenderDataRenewed, std::shared_mutex &newestRenderDataMutex) {
        while (!WindowShouldClose()) {
            InputData newInputData = game::input::player_input_get(debugConfiguration);
            {
                std::unique_lock inputDataLock(nextInputDataMutex);
                nextInputData = std::move(newInputData);
                nextInputDataRenewed = true;
            }
            rendering::draw(constantRenderData, debugConfiguration, currentRenderData);
            
            {
                std::shared_lock newestRenderDataLock(newestRenderDataMutex);
                if (newestRenderDataRenewed) {
                    currentRenderData = std::move(newestRenderData);
                    newestRenderDataRenewed = false;
                }
            }
        }
    }
}

void initialize_player(GameData &gameData) {
    auto playerEntity = gameData.registry.create();
    gameData.registry.emplace<PlayerMarker>(playerEntity);
    gameData.registry.emplace<Position>(playerEntity, gameData.map.tile_size() * 2, gameData.map.tile_size() * 2);
    gameData.registry.emplace<BodySize>(playerEntity, gameData.map.tile_size(), gameData.map.tile_size());
    gameData.registry.emplace<BodyRotation>(playerEntity, 70);
    gameData.registry.emplace<Acceleration>(playerEntity, 1500);
    gameData.registry.emplace<HitBoxRadius>(playerEntity, gameData.map.tile_size() / 2.5);
    gameData.registry.emplace<SpeedVector>(playerEntity, 0, 0);
    gameData.registry.emplace<SpecificFloorFrictionSlowdown>(playerEntity, 2);
}

void initialize_test_weapon(GameData &gameData) {
    auto weaponEntity = gameData.registry.create();
    gameData.registry.emplace<WeaponMarker>(weaponEntity);
    gameData.registry.emplace<PickUpMarker>(weaponEntity);
    gameData.registry.emplace<Position>(weaponEntity, gameData.map.tile_size() * 4, gameData.map.tile_size() * 4);
    gameData.registry.emplace<BodySize>(weaponEntity, gameData.map.tile_size() / 2, gameData.map.tile_size() / 2);
    gameData.registry.emplace<HitBoxRadius>(weaponEntity, gameData.map.tile_size() / 5);
    gameData.registry.emplace<BodyRotation>(weaponEntity, 0);
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
    GameData gameData(640, 360);
    initialize_player(gameData);
    initialize_map(gameData);
    initialize_test_weapon(gameData);
    std::shared_mutex newestInputDataMutex;
    InputData newestInputData;
    bool newestInputDataRenewed;
    InitWindow(gameData.worldWidth, gameData.worldHeight, "hlmrl");
    ConstantRenderData constantRenderData(gameData.worldWidth, gameData.worldHeight);
    RenderData currentRenderData;
    std::shared_mutex newestRenderDataMutex;
    bool newestRenderDataRenewed = false;
    RenderData newestRenderData;
    game::PhysicsManagement physicsManagement(gameData, debugConfiguration, newestRenderData, newestRenderDataRenewed, newestRenderDataMutex, newestInputData, newestInputDataRenewed, newestInputDataMutex, 640);
    SetTargetFPS(0);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowSize(1280, 720);
    if (debugConfiguration.logLevel >= LogLevel::DEBUG)
        fmt::println("Entering game-loop");
    physicsManagement.start();
    game::entry(debugConfiguration, newestInputData, newestInputDataRenewed, newestInputDataMutex, constantRenderData, currentRenderData, newestRenderData, newestRenderDataRenewed, newestRenderDataMutex);
    if (debugConfiguration.logLevel >= LogLevel::DEBUG)
        fmt::println("Closing and unloading game");
    CloseWindow();
    std::unique_lock runningLock(gameData.runningMutex);
    gameData.running = false;
    runningLock.unlock();
    if (debugConfiguration.logLevel >= LogLevel::DEBUG)
        fmt::println("Unloaded everything");
    return 0;
}
