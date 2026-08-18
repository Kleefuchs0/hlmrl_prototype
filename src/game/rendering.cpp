#include "game/rendering.hpp"
#include "game/ConstantRenderData.hpp"
#include "game/RenderData.hpp"
#include "lib/BodyRotationMutex.hpp"
#include "lib/DebugConfiguration.hpp"
#include "lib/HitBoxRadius.hpp"
#include "lib/PlayerMarker.hpp"
#include "lib/PositionMutex.hpp"
#include "raylib.h"
#include <entt/entity/fwd.hpp>
#include <fmt/base.h>
#include <stddef.h>

using namespace game;

namespace game {
    namespace rendering {
        namespace internal {
            Camera2D makeCamera(const GameData &gameData, const int worldWidth, int worldHeight) {
                const auto &playerView = gameData.registry.view<PlayerMarker, Position>();
                for (const entt::entity &player : playerView) {
                    const Position &pos = gameData.registry.get<Position>(player);
                    return {
                        {static_cast<float>(worldWidth) / 2, static_cast<float>(worldHeight) / 2},
                        pos,
                        0,
                        1,
                    };
                }
                return {};
            }

            template <size_t MAP_WIDTH, size_t MAP_HEIGHT, float TILE_SIZE>
                RenderLayer makeMapLayer(const Map<MAP_WIDTH, MAP_HEIGHT, TILE_SIZE> &map, const Camera2D &cam, int worldWidth, int worldHeight) {
                    RenderLayer layer;

                    std::pair<Position, Position> worldScreenWindowRange;
                    worldScreenWindowRange.first = GetScreenToWorld2D({0, 0}, cam);
                    worldScreenWindowRange.second = GetScreenToWorld2D({static_cast<float>(worldWidth), static_cast<float>(worldHeight)}, cam);

                    std::pair<std::pair<size_t, size_t>, std::pair<size_t, size_t>> tileRanges;
                    tileRanges.first = {worldScreenWindowRange.first.x / TILE_SIZE, worldScreenWindowRange.first.y / TILE_SIZE};
                    tileRanges.second = {std::ceil(worldScreenWindowRange.second.x / TILE_SIZE) + 1, std::ceil(worldScreenWindowRange.second.y / TILE_SIZE) + 1};

                    if (tileRanges.first.first >= MAP_WIDTH)     // Correct too big values
                        tileRanges.first.first = 0;
                    if (tileRanges.first.second >= MAP_HEIGHT)     // Correct too big values
                        tileRanges.first.second = 0;
                    // if (tileRanges.second.first >= MAP_WIDTH)     // Correct too big values
                    //     tileRanges.second.first = 1;
                    // if (tileRanges.second.second >= MAP_HEIGHT)     // Correct too big values
                    //     tileRanges.second.second = 1;


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
                            layer.objects.push_back({ {0, 0, static_cast<uint16_t>(TILE_SIZE), static_cast<uint16_t>(TILE_SIZE)}, color, {x * TILE_SIZE, y * TILE_SIZE}, {TILE_SIZE, TILE_SIZE}, 0});
                        }
                    }
                    return layer;
                }

            namespace singular {
                RenderObject makeEntity([[maybe_unused]]const GameData &gameData, [[maybe_unused]] const DebugConfiguration &debugConfiguration, const Position &position, PositionMutex &positionMutex, const BodySize &bodySize, const BodyRotation &bodyRotation, BodyRotationMutex &bodyRotationMutex) {
                    std::shared_lock positionLock(positionMutex);
                    std::shared_lock bodyRotationLock(bodyRotationMutex);
                    return {{0, 0, static_cast<uint16_t>(bodySize.x), static_cast<uint16_t>(bodySize.y)}, WHITE, {position.x, position.y}, {bodySize.x / 2, bodySize.y / 2}, bodyRotation.value()};
                }
            }

            RenderLayer makeEntityLayer(GameData &gameData, DebugConfiguration &debugConfiguration) {
                RenderLayer renderLayer;
                entt::registry &registry = gameData.registry;
                auto entity_view = gameData.registry.view<Position, PositionMutex, BodySize, HitBoxRadius, BodyRotation, BodyRotationMutex>();
                for (const entt::entity &entity : entity_view) {
                    const auto &[position, positionMutex, bodySize, bodyRotation, bodyRotationMutex, hitBoxRadius] = registry.get<Position, PositionMutex, BodySize, BodyRotation, BodyRotationMutex, HitBoxRadius>(entity);
                    renderLayer.objects.push_back(singular::makeEntity(gameData, debugConfiguration, position, positionMutex, bodySize, bodyRotation, bodyRotationMutex));
                }
                return renderLayer;
            }
        }
    }
}

RenderData rendering::internal::process_game_data_to_render_data(GameData &gameData, [[maybe_unused]] DebugConfiguration &debugConfiguration) {
    RenderData renderData;
    {
        std::shared_lock worldSizeLock(gameData.worldSizeMutex);
        renderData.worldWidth = gameData.worldWidth;
        renderData.worldHeight = gameData.worldHeight;
    }
    renderData.cam = rendering::internal::makeCamera(gameData, renderData.worldWidth, renderData.worldHeight);
    renderData.backgroundColor = BLUE;
    renderData.screenTint = WHITE;
    {
        std::shared_lock mapLock(gameData.mapMutex);
        renderData.layers.push_back(makeMapLayer(gameData.map, renderData.cam, renderData.worldWidth, renderData.worldHeight));
    }
    std::shared_lock registryLock(gameData.registryMutex);
    renderData.layers.push_back(makeEntityLayer(gameData, debugConfiguration));
    return renderData;
}

void rendering::draw(ConstantRenderData &constantRenderData, DebugConfiguration &debugConfiguration, const RenderData &renderData) {
    BeginDrawing();
    ClearBackground(BLACK);
    BeginTextureMode(constantRenderData.renderTexture);
    BeginMode2D(renderData.cam);
    ClearBackground(renderData.backgroundColor);
    for (const RenderLayer &layer : renderData.layers) {
        for (const RenderObject &object : layer.objects) {
            DrawRectanglePro({object.pos.x, object.pos.y, object.size.x, object.size.y}, {object.size.x / 2, object.size.y / 2}, object.rotation.value(), object.tint);
        }
    }
    EndMode2D();
    if (debugConfiguration.drawFPS)
        DrawFPS(10, 10);
    EndTextureMode();
    DrawTexturePro(constantRenderData.renderTexture.texture, {0, 0, static_cast<float>(constantRenderData.renderTexture.texture.width), -static_cast<float>(constantRenderData.renderTexture.texture.height)}, {0, 0, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())}, {0, 0}, 0.0f, renderData.screenTint);
    EndDrawing();
}
