#include "game_draw.hpp"
#include "BodyRotation.hpp"
#include "BodyRotationMutex.hpp"
#include "DebugConfiguration.hpp"
#include "HitBoxRadius.hpp"
#include "Position.hpp"
#include "BodySize.hpp"
#include "PositionMutex.hpp"
#include "raylib.h"
#include <cstddef>
#include <cstdlib>
#include <shared_mutex>
#include <stddef.h>
#include <utility>

using namespace game::loop;

template<size_t MAP_WIDTH, size_t MAP_HEIGHT, float TILE_SIZE>
void draw::draw_map(Map<MAP_WIDTH, MAP_HEIGHT, TILE_SIZE> &map, GameData &gameData) {

    std::pair<Position, Position> worldScreenWindowRange;
    std::shared_lock worldSizeLock(gameData.worldSizeMutex);
    worldScreenWindowRange.first = GetScreenToWorld2D({0, 0}, gameData.cam);
    worldScreenWindowRange.second = GetScreenToWorld2D({static_cast<float>(gameData.worldWidth), static_cast<float>(gameData.worldHeight)}, gameData.cam);
    worldSizeLock.unlock();

    std::pair<std::pair<size_t, size_t>, std::pair<size_t, size_t>> tileRanges;
    tileRanges.first = {worldScreenWindowRange.first.x / TILE_SIZE, worldScreenWindowRange.first.y / TILE_SIZE};
    tileRanges.second = {std::ceil(worldScreenWindowRange.second.x / TILE_SIZE), std::ceil(worldScreenWindowRange.second.y / TILE_SIZE)};

    if (tileRanges.first.first >= MAP_WIDTH)     // Correct too big values
        tileRanges.first.first = 0;
    if (tileRanges.first.second >= MAP_HEIGHT)     // Correct too big values
        tileRanges.first.second = 0;
    // if (tileRanges.second.first >= MAP_WIDTH)     // Correct too big values
    //     tileRanges.second.first = 1;
    // if (tileRanges.second.second >= MAP_HEIGHT)     // Correct too big values
    //     tileRanges.second.second = 1;

    
    std::shared_lock mapLock(gameData.mapMutex);
    for (size_t y : std::views::iota(tileRanges.first.second, tileRanges.second.second)) {
        for (size_t x : std::views::iota(tileRanges.first.first, tileRanges.second.first)) {
            TileType tileType = map.get_tile_type(x, y);
            Color color = WHITE;
            switch (tileType.value()) {
                case tile_type::EMPTY:
                    continue;           // Skip empty ones
                case tile_type::WALL:
                    color = GRAY;
                    break;
                case tile_type::FLOOR:
                    color = DARKBLUE;
                    break;
            }
            DrawRectanglePro({static_cast<float>(x * TILE_SIZE), static_cast<float>(y * TILE_SIZE), TILE_SIZE, TILE_SIZE}, {0, 0}, 0, color);
        }
    }
}

void draw::draw_entity([[maybe_unused]]const GameData &gameData, const DebugConfiguration &debugConfiguration, const Position &position, PositionMutex &positionMutex, const BodySize &bodySize, const BodyRotation &bodyRotation, BodyRotationMutex &bodyRotationMutex, const HitBoxRadius &hitBoxRadius) {
    std::shared_lock positionLock(positionMutex);
    std::shared_lock bodyRotationLock(bodyRotationMutex);
    DrawRectanglePro({position.x, position.y, bodySize.x, bodySize.y}, {bodySize.x / 2, bodySize.y / 2}, bodyRotation.value(), WHITE);
    bodyRotationLock.unlock();
    if (debugConfiguration.drawHitBoxes)
        DrawCircleV(position, hitBoxRadius.value(), debugConfiguration.hitBoxColor);
}

void draw::draw_entities(GameData &gameData, DebugConfiguration &debugConfiguration) {
    std::shared_lock registryLock(gameData.registryMutex);
    entt::registry &registry = gameData.registry;
    auto entity_view = gameData.registry.view<Position, PositionMutex, BodySize, HitBoxRadius, BodyRotation, BodyRotationMutex>();
    for (const entt::entity &entity : entity_view) {
        const auto &[position, positionMutex, bodySize, bodyRotation, bodyRotationMutex, hitBoxRadius] = registry.get<Position, PositionMutex, BodySize, BodyRotation, BodyRotationMutex, HitBoxRadius>(entity);
        draw_entity(gameData, debugConfiguration, position, positionMutex, bodySize, bodyRotation, bodyRotationMutex, hitBoxRadius);
    }
}

void draw::draw(GameData &gameData, DebugConfiguration &debugConfiguration) {
    BeginDrawing();
    ClearBackground(BLACK);
    BeginTextureMode(gameData.renderTexture);
    std::shared_lock camLock(gameData.camMutex);
    BeginMode2D(gameData.cam);
    ClearBackground(gameData.backgroundColor);
    draw_map(gameData.map, gameData);
    draw_entities(gameData, debugConfiguration);
    EndMode2D();
    if (debugConfiguration.drawFPS)
        DrawFPS(10, 10);
    EndTextureMode();
    DrawTexturePro(gameData.renderTexture.texture, {0, 0, static_cast<float>(gameData.renderTexture.texture.width), -static_cast<float>(gameData.renderTexture.texture.height)}, {0, 0, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())}, {0, 0}, 0.0f, gameData.screenTint);
    EndDrawing();
}

