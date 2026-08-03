#include "GameData.hpp"
#include "Player.hpp"
#include "Position.hpp"
#include "game_tick.hpp"
#include "TickedFunction.hpp"
#include "game_draw.hpp"
#include <raylib.h>
#include <raymath.h>

#define TILE_SIZE 64

namespace game {

    namespace loop {

        void input_update(GameData &gameData) {
            Player &player = gameData.player;
            if(IsKeyDown(KEY_W)) {
                player.pos.y -= 2;
            }
            if(IsKeyDown(KEY_A)) {
                player.pos.x -= 2;
            }
            if(IsKeyDown(KEY_S)) {
                player.pos.y += 2;
            }
            if(IsKeyDown(KEY_D)) {
                player.pos.x += 2;
            }
        }

        void entry(GameData &gameData) {
            while (!WindowShouldClose()) {
                tick::tick_update(gameData);
                draw::draw(gameData);
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
    gameData.player.size.x = TILE_SIZE;
    gameData.player.size.y = TILE_SIZE;
    gameData.player.pos.x = 0;
    gameData.player.pos.y = 0;
    gameData.player.rotation = 70;
}

int main() {
    GameData gameData(generate_default_cam(640, 360), 640, 360);
    gameData.tickedFunctions["input"] = TickedFunction(1, &game::loop::input_update);
    initialize_player(gameData);
    InitWindow(gameData.worldWidth, gameData.worldHeight, "hlmrl");
    SetTargetFPS(1000);
    game::loop::entry(gameData);
    CloseWindow();
    return 0;
}
