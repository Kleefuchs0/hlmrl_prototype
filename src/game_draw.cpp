#include "game_draw.hpp"
#include "constants.hpp"
#include "raylib.h"
#include <cstdlib>

using namespace game::loop;

void draw::draw_map(GameData &gameData) {
    Map<DEFAULT_MAP_WIDTH, DEFAULT_MAP_HEIGHT> &map = gameData.map;
    
    for (size_t y = 0; y < map.height(); y++) {
        for (size_t x = 0; x < map.width(); x++) {
            Color color;
            color.a = 1;
            color.r = random();
            color.b = random();
            color.g = random();
            DrawRectanglePro({static_cast<float>(x * TILE_SIZE), static_cast<float>(y * TILE_SIZE), TILE_SIZE, TILE_SIZE}, {0, 0}, 0, color);
        }
    }
}

void draw::draw_player(GameData &gameData) {
    Player &player = gameData.player;
    DrawRectanglePro({player.pos.x, player.pos.y, player.size.x, player.size.y}, {player.size.x / 2, player.size.y / 2}, player.rotation, WHITE);
}

void draw::draw(GameData &gameData) {
    BeginDrawing();
    ClearBackground(BLACK);
    BeginMode2D(gameData.cam);
    draw_map(gameData);
    draw_player(gameData);
    EndMode2D();
    EndDrawing();
}

