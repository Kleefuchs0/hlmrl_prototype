#include "GameData.hpp"
#include "Player.hpp"
#include "Position.hpp"
#include "TickedFunction.hpp"
#include <raylib.h>
#include <raymath.h>

#define TILE_SIZE 64

namespace game {

    namespace loop {
        void tickedEngineFunctionsUpdate(GameData &gameData) {
            for (auto it = gameData.tickedFunctions.begin(); it != gameData.tickedFunctions.end(); it++) {
                TickedFunction &tickedEngineFunction = *it.base();
                if (gameData.tick % tickedEngineFunction.tickGoal == 0) {
                    tickedEngineFunction.function(gameData);
                }
            }
        }

        void tickUpdate(GameData &gameData) {
            gameData.tickClock += GetFrameTime();
            double tickTime = 1.0 / gameData.tickRate;
            if (gameData.tickClock < tickTime) {
                return;
            }
            for (size_t i = 1; i < gameData.tickClock / tickTime; i++) {
                gameData.tick++;
                tickedEngineFunctionsUpdate(gameData);
            }
            gameData.tickClock = 0;
        }


        void draw_map(GameData &gameData, int worldWidth, int worldHeight) {
            Player player = gameData.player;
            Position startingPoint = {player.pos.x - static_cast<float>(worldWidth) / 2, player.pos.y - static_cast<float>(worldHeight) / 2};
            Position endPoint = {player.pos.x + static_cast<float>(worldWidth) / 2, player.pos.y + static_cast<float>(worldHeight) / 2};

        }

        void draw_player(GameData &gameData) {
            Player &player = gameData.player;
            DrawRectanglePro({player.pos.x, player.pos.y, player.size.x, player.size.y}, {player.size.x / 2, player.size.y / 2}, player.rotation, WHITE);
        }

        void draw(GameData &gameData) {
            BeginDrawing();
            ClearBackground(BLACK);
            BeginMode2D(gameData.cam);
            draw_map(gameData, gameData.worldWidth, gameData.worldHeight);
            draw_player(gameData);
            EndMode2D();
            EndDrawing();
        }

        void entry(GameData &gameData) {
            while (!WindowShouldClose()) {
                draw(gameData);
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
    initialize_player(gameData);
    InitWindow(gameData.worldWidth, gameData.worldHeight, "hlmrl");
    game::loop::entry(gameData);
    CloseWindow();
    return 0;
}
