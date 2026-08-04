#include "game_draw.hpp"
#include "DebugConfiguration.hpp"
#include "constants.hpp"
#include "raylib.h"
#include <cstddef>
#include <cstdlib>
#include <stddef.h>
#include <utility>

using namespace game::loop;

template<size_t MAP_WIDTH, size_t MAP_HEIGHT>
void draw::draw_map(Player &player, Map<MAP_WIDTH, MAP_HEIGHT> &map, GameData &gameData) {

    std::pair<Position, Position> worldScreenWindowRange;
    worldScreenWindowRange.first = GetScreenToWorld2D({0, 0}, gameData.cam);
    worldScreenWindowRange.second = GetScreenToWorld2D({static_cast<float>(gameData.worldWidth), static_cast<float>(gameData.worldHeight)}, gameData.cam);

    std::pair<std::pair<size_t, size_t>, std::pair<size_t, size_t>> tileRanges;
    tileRanges.first = {worldScreenWindowRange.first.x / TILE_SIZE, worldScreenWindowRange.first.y / TILE_SIZE};
    tileRanges.second = {std::ceil(worldScreenWindowRange.second.x / TILE_SIZE), std::ceil(worldScreenWindowRange.second.y / TILE_SIZE)};

    if (tileRanges.first.first >= MAP_WIDTH)     // Correct too big values
        tileRanges.first.first = 0;
    if (tileRanges.first.second >= MAP_HEIGHT)     // Correct too big values
        tileRanges.first.second = 0;

    
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

void draw::draw_player(GameData &gameData, DebugConfiguration &debugConfiguration) {
    Player &player = gameData.player;
    DrawRectanglePro({player.pos.x, player.pos.y, player.size.x, player.size.y}, {player.size.x / 2, player.size.y / 2}, player.rotation, WHITE);
    if (debugConfiguration.drawHitBoxes)
        DrawCircleV(player.pos, player.circularHitBoxRadius, debugConfiguration.hitBoxColor);
}

void draw::draw(GameData &gameData, DebugConfiguration &debugConfiguration) {
    BeginDrawing();
    ClearBackground(BLACK);
    BeginTextureMode(gameData.renderTexture);
    BeginMode2D(gameData.cam);
    ClearBackground(gameData.backgroundColor);
    draw_map(gameData.player, gameData.map, gameData);
    draw_player(gameData, debugConfiguration);
    EndMode2D();
    if (debugConfiguration.drawFPS)
        DrawFPS(10, 10);
    EndTextureMode();
    DrawTexturePro(gameData.renderTexture.texture, {0, 0, static_cast<float>(gameData.renderTexture.texture.width), -static_cast<float>(gameData.renderTexture.texture.height)}, {0, 0, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())}, {0, 0}, 0.0f, gameData.screenTint);
    EndDrawing();
}

