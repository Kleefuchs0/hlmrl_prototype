#include "game_draw.hpp"

using namespace game::loop;

void draw::draw_map(const GameData &gameData) {
    Player player = gameData.player;
    Position startingPoint = {player.pos.x - static_cast<float>(gameData.worldWidth) / 2, player.pos.y - static_cast<float>(gameData.worldHeight) / 2};
    Position endPoint = {player.pos.x + static_cast<float>(gameData.worldWidth) / 2, player.pos.y + static_cast<float>(gameData.worldHeight) / 2};

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

