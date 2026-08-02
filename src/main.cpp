#include "GameData.hpp"
#include <raylib.h>

void game_loop_draw(GameData &gameData) {
    BeginDrawing();
    ClearBackground(BLACK);
    BeginMode2D(gameData.cam);
    EndMode2D();
    EndDrawing();
}

void game_loop(GameData &gameData) {
    while (!WindowShouldClose()) {
        game_loop_draw(gameData);
    }
}

Camera2D generate_default_cam() {
    Camera2D cam;
    cam.offset = {0, 0};
    cam.rotation = 0;
    cam.target = {0, 0};
    cam.zoom = 1;
    return cam;
}

int main() {
    GameData gameData(generate_default_cam());
    InitWindow(640, 360, "hlmrl");
    game_loop(gameData);
    CloseWindow();
    return 0;
}
