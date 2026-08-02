#include "GameData.hpp"
#include "Player.hpp"
#include "Position.hpp"
#include <raylib.h>

#define TILE_SIZE 64

void game_loop_draw_map(GameData &gameData) {
}

void game_loop_draw_player(GameData &gameData) {
    Player &player = gameData.player;
    DrawRectanglePro({player.pos.x, player.pos.y, player.size.x, player.size.y}, {player.size.x / 2, player.size.y / 2}, player.rotation, WHITE);
}

void game_loop_draw(GameData &gameData) {
    BeginDrawing();
    ClearBackground(BLACK);
    BeginMode2D(gameData.cam);
    game_loop_draw_map(gameData);
    game_loop_draw_player(gameData);
    EndMode2D();
    EndDrawing();
}

void game_loop(GameData &gameData) {
    while (!WindowShouldClose()) {
        game_loop_draw(gameData);
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
    gameData.player.size.x = TILE_SIZE;
    gameData.player.size.y = TILE_SIZE;
    gameData.player.pos.x = 0;
    gameData.player.pos.y = 0;
    gameData.player.rotation = 70;
}

int main() {
    GameData gameData(generate_default_cam(640, 360));
    initialize_player(gameData);
    InitWindow(640, 360, "hlmrl");
    game_loop(gameData);
    CloseWindow();
    return 0;
}
