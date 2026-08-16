#include "Acceleration.hpp"
#include "BodyRotation.hpp"
#include "DebugConfiguration.hpp"
#include "PickUpMarker.hpp"
#include "SpecificFloorFrictionSlowdown.hpp"
#include "SpeedVector.hpp"
#include "EVector2.hpp"
#include "GameData.hpp"
#include "HitBoxRadius.hpp"
#include "LogLevel.hpp"
#include "PlayerMarker.hpp"
#include "Position.hpp"
#include "BodySize.hpp"
#include "WeaponMarker.hpp"
#include "constants.hpp"
#include "entity_map_interaction.hpp"
#include "entt/entity/fwd.hpp"
#include "fmt/core.h"
#include "game_tick.hpp"
#include "TickedFunction.hpp"
#include "game_draw.hpp"
#include "tile_type.hpp"
#include <cmath>
#include <cstddef>
#include <raylib.h>
#include <raymath.h>
#include "entity_try_move.hpp"

namespace game {

    namespace loop {


        float get_player_angle_to_mouse(const GameData &gameData, const Position &playerPosition) {
            Position mousePositionRelative = GetScreenToWorld2D(GetMousePosition() / (static_cast<float>(GetScreenWidth()) / static_cast<float>(gameData.worldWidth)), gameData.cam) - playerPosition;
            return static_cast<float>(std::atan2(static_cast<double>(mousePositionRelative.y), static_cast<double>(mousePositionRelative.x))) * static_cast<float>(180 / M_PI);
        }

        void player_input_update_speedvector(SpeedVector &deltaSpeed, const Acceleration &acceleration) {

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

            deltaSpeed += playerDeltaSpeedChangeVector;
        }

        void player_input_update(const entt::entity &player, GameData &gameData, [[maybe_unused]] DebugConfiguration &debugConfiguration) {
            const auto &[bodyRotation, position, speedVector, acceleration] = gameData.registry.get<BodyRotation, Position, SpeedVector, Acceleration>(player);
            bodyRotation = get_player_angle_to_mouse(gameData, position);

            player_input_update_speedvector(speedVector, acceleration);
        }

        void players_input_update(GameData &gameData, DebugConfiguration &debugConfiguration) {
            auto playerView = gameData.registry.view<PlayerMarker, SpeedVector, BodyRotation, Acceleration>();
            for (const entt::entity &player : playerView)
                player_input_update(player, gameData, debugConfiguration);
        }

        void player_update(const entt::entity &player, GameData &gameData, DebugConfiguration &debugConfiguration) {
            //TODO: Implement bounds checking for player.
            const auto &[speedVector, position, hitBoxRadius] = gameData.registry.get<SpeedVector, Position, HitBoxRadius>(player);
            try_move_entity_with_deltaSpeed_change_on_collision(position, speedVector, hitBoxRadius, gameData.map, speedVector, debugConfiguration);

            gameData.cam.target = position;
        }

        void players_update(GameData &gameData, DebugConfiguration &debugConfiguration) {
            auto playerView = gameData.registry.view<PlayerMarker, Position, SpeedVector, Acceleration, HitBoxRadius, BodyRotation>();
            for (const entt::entity &player : playerView)
                player_update(player, gameData, debugConfiguration);
        }

        void entity_update_floor_friction(GameData &gameData, const DebugConfiguration &debugConfiguration, Position &position, SpeedVector &speedVector, HitBoxRadius &hitBoxRadius, SpecificFloorFrictionSlowdown &specificFloorFrictionSlowdown) {
            std::array<TileType, 4> tiles = get_map_collision_tiles(position, hitBoxRadius, gameData.map, debugConfiguration);
            for (TileType tile : tiles) {
                if (tile.value() >= TILE_TYPE_SECTION_START_FLOORS && tile.value() < TILE_TYPE_SECTION_START_WALLS) {
                    speedVector /= 1 + specificFloorFrictionSlowdown.value();
                }
            }
        }

        void entities_update_friction(GameData &gameData, DebugConfiguration &debugConfiguration) {
            auto entityView = gameData.registry.view<Position, SpeedVector, HitBoxRadius, SpecificFloorFrictionSlowdown>();
            for (const entt::entity &entity : entityView) {
                const auto &[position, speedVector, hitBoxRadius, specificFloorFritionSlowdown] = gameData.registry.get<Position, SpeedVector, HitBoxRadius, SpecificFloorFrictionSlowdown>(entity);
                entity_update_floor_friction(gameData, debugConfiguration, position, speedVector, hitBoxRadius, specificFloorFritionSlowdown);
            }
        }

        void pickup_update(BodyRotation &bodyRotation) {
            bodyRotation += 0.5f;
        }

        void pickups_update(GameData &gameData, [[maybe_unused]] DebugConfiguration &debugConfiguration) {
            auto weaponView = gameData.registry.view<PickUpMarker, BodyRotation>();
            for (const entt::entity &weapon : weaponView) {
                BodyRotation &bodyRotation = gameData.registry.get<BodyRotation>(weapon);
                pickup_update(bodyRotation);
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
    gameData.registry.emplace<BodySize>(playerEntity, gameData.map.tile_size(), gameData.map.tile_size());
    gameData.registry.emplace<BodyRotation>(playerEntity, 70);
    gameData.registry.emplace<Acceleration>(playerEntity, 0.2);
    gameData.registry.emplace<HitBoxRadius>(playerEntity, gameData.map.tile_size() / 2.5);
    gameData.registry.emplace<SpeedVector>(playerEntity, 0, 0);
    gameData.registry.emplace<SpecificFloorFrictionSlowdown>(playerEntity, .02);
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
    GameData gameData(generate_default_cam(640, 360), 640, 360);
    gameData.tickRate = 128;
    initialize_player(gameData);
    initialize_map(gameData);
    initialize_test_weapon(gameData);
    gameData.tickedFunctions["players_input_update"] = TickedFunction(1, &game::loop::players_input_update);
    gameData.tickedFunctions["players_update"] = TickedFunction(1, &game::loop::players_update);
    gameData.tickedFunctions["entities_friction_update"] = TickedFunction(1, &game::loop::entities_update_friction);
    gameData.tickedFunctions["pickups_update"] = TickedFunction(1, &game::loop::pickups_update);
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
