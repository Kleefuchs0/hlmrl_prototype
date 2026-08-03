#include "EVector2.hpp"
#include "GameData.hpp"
#include "Player.hpp"
#include "Position.hpp"
#include "game_tick.hpp"
#include "TickedFunction.hpp"
#include "game_draw.hpp"
#include <functional>
#include <raylib.h>
#include <raymath.h>
#include <thread>

#define TILE_SIZE 64

namespace game {

    namespace loop {

        void input_update(GameData &gameData) {
            Player &player = gameData.player;
            if(IsKeyPressed(KEY_F11)) {
                ToggleFullscreen();
            }
            EVector2 calculatedVector = {0, 0};
            if(IsKeyDown(KEY_W)) {
                calculatedVector.y -= 1;
            }
            if(IsKeyDown(KEY_S)) {
                calculatedVector.y += 1;
            }
            if(IsKeyDown(KEY_A)) {
                calculatedVector.x -= 1;
            }
            if(IsKeyDown(KEY_D)) {
                calculatedVector.x += 1;
            }
            float angle = std::atan2(calculatedVector.y, calculatedVector.x);
            player.pos.x += cos(angle) * player.speed.value();
            player.pos.y += sin(angle) * player.speed.value();
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
